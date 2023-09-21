#include "../include/KqueueLoop.hpp"
#include "../include/CgiHandler.hpp"
#include "../include/SocketConnect.hpp"
#include "../include/SocketListen.hpp"
#include "../include/Response.hpp"
#include "../include/macro.hpp"

KqueueLoop::KqueueLoop(std::vector<Server> *servers, int kq) : _kq_main(kq), _n_ev(0), _servers(servers)
{
	// clean the kevent to catch the event
	for (int i = 0; i < TOTAL_KEV; i++)
		EV_SET(&_kev_catch[i], -1, EVFILT_READ, EV_DISABLE, 0, 0, NULL);

	// set _listListeningSocketInt (list to check the listening socket number)
	for (int i = 0; i < (int)_servers->size(); i++)
	{
		std::vector<Server> &temp = *_servers;
		Server &tempserver = temp[i];
		std::vector<SocketListen> *tempsocketlist = tempserver.getSocketListen();

		for (int i = 0; i < (int)tempsocketlist->size(); i++)
		{
			std::vector<SocketListen>::iterator itr = tempsocketlist->begin() + i;
			_listListeningSocketInt.push_back(itr->getNumSocket());
		}
	}
}

KqueueLoop::~KqueueLoop()
{
}

int KqueueLoop::checkListeningSocket(int sock)
{
	for (int i = 0; i < (int)_listListeningSocketInt.size(); i++)
	{
		if (_listListeningSocketInt[i] == sock)
			return (sock);
	}
	return (-1);
}

static int getEventIndexFromSocketsList(std::vector<SocketConnect *> socketConnects, int eventFd)
{
	// std::cout << "socketConnects.size() is " << socketConnects.size() << std::endl;
	for (int i = 0; i < (int)socketConnects.size(); i++)
	{
		if (socketConnects[i]->getNumSocket() == eventFd)
		{
			return i;
		}
	}
	return -1;
}

void addKqFilter(int kqMain, int filter, int flags, int eventFd)
{
	struct kevent _kev_catch;

	EV_SET(&_kev_catch, eventFd, filter, flags, 0, 0, nullptr);
	kevent(kqMain, &_kev_catch, 1, nullptr, 0, nullptr);
}

int KqueueLoop::startLoop()
{
	std::cout << "Kqueue start" << std::endl;
	std::vector<SocketConnect *> socketConnects;

	while (1)
	{
		_n_ev = kevent(_kq_main, NULL, 0, _kev_catch, TOTAL_KEV, NULL);
		for (int i = 0; i < _n_ev; i++)
		{
			try
			{
				if (_kev_catch[i].flags & EV_EOF || _kev_catch[i].flags & EV_ERROR)
				{
					std::cout << "closed!" << std::endl;
					throw std::invalid_argument("error flag on a socket");
				}
				int socket_num = checkListeningSocket(_kev_catch[i].ident);
				if (socket_num > 2)
				{
					// std::cout << std::endl << "[Event on listenning socket] " << socket_num << std::endl;
					SocketConnect *newSocket = new SocketConnect((int)_kev_catch[i].ident, _kq_main, _servers);
					if (newSocket == NULL)
						throw std::invalid_argument("error on accepting new socket");
					socketConnects.push_back(newSocket);
					continue;
				}

				int where_socket = getEventIndexFromSocketsList(socketConnects, (int)_kev_catch[i].ident);
				if (where_socket < 0)
					throw std::invalid_argument("Socket not found");
				SocketConnect *currentsocket = socketConnects[where_socket];
				if (_kev_catch[i].filter == EVFILT_READ && !(_kev_catch[i].flags & EV_ERROR))
				{
					// std::cout << std::endl << "[READ Event on connection socket(EVFILT_READ)] " << _kev_catch[i].ident << std::endl;
					if (currentsocket->readRequest() == BUFFSIZE)
					{
						EV_SET(&_kev_catch[i], _kev_catch[i].ident, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
						kevent(_kq_main, &_kev_catch[i], 1, NULL, 0, NULL);
						continue;
					}
					if (currentsocket->getClientRequest()->getSizeR() == 0)
						throw std::invalid_argument("Request is empty");
					currentsocket->setRequest(_servers);
					currentsocket->getClientResponse()->makeResponse(currentsocket->getClientRequest(), currentsocket);
					EV_SET(&_kev_catch[i], _kev_catch[i].ident, EVFILT_READ, EV_DELETE, 0, 0, 0);
					EV_SET(&_kev_catch[i], _kev_catch[i].ident, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, 0);
					kevent(_kq_main, &_kev_catch[i], 1, NULL, 0, NULL);
				}
				else if (_kev_catch[i].filter == EVFILT_WRITE && !(_kev_catch[i].flags & EV_ERROR)) // check if the socket is to write
				{
					if (currentsocket->doRedirect(socketConnects, where_socket))
					{

						throw std::invalid_argument("Redirect is done");
					}
					if (currentsocket->getClientResponse()->sendResponse(currentsocket->getNumSocket()))
					{
						EV_SET(&_kev_catch[i], _kev_catch[i].ident, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, 0);
						kevent(_kq_main, &_kev_catch[i], 1, NULL, 0, NULL);
						continue;
					}
					socketConnects.erase(socketConnects.begin() + where_socket);
					delete (currentsocket);
					close(_kev_catch[i].ident);
				}
			}
			catch (std::exception &e) // we must leave the error without using exit (otherwise the server stops for one error on a stream)
			{
				close(_kev_catch[i].ident);
				std::cout << e.what() << std::endl;
				int num_socket_registered = getEventIndexFromSocketsList(socketConnects, _kev_catch[i].ident);
				if (num_socket_registered >= 0)
				{
					delete (socketConnects[num_socket_registered]);
					socketConnects.erase(socketConnects.begin() + num_socket_registered);
				}
			}
		}
	}
	return (0);
}

// getter

int KqueueLoop::getKqMain()
{
	return (_kq_main);
}

// exception
KqueueLoop::ERR_KqueueLoop::ERR_KqueueLoop() : _error_msg("KqueueLoop setting failed") {}
KqueueLoop::ERR_KqueueLoop::ERR_KqueueLoop(const char *error_msg) : _error_msg(error_msg) {}

const char *KqueueLoop::ERR_KqueueLoop::what() const _NOEXCEPT
{
	return (_error_msg);
}
