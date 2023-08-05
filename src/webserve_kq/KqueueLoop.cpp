//
// KqueueLoop
//

#include "../../include/KqueueLoop.hpp"
#include "../../include/SocketConnect.hpp"
#include <exception>

KqueueLoop::KqueueLoop(std::vector<Server> *servers, int kq): _kq_main(kq), _n_ev(0), _servers(servers)
{
	// clean the kevent to catch the event
	for (int i = 0; i < TOTAL_KEV; i++)
		EV_SET(&_kev_catch[i], -1, EVFILT_READ, EV_DISABLE, 0, 0, NULL);

	// set _listListeningSocketInt (list to check the listening socket number)
	for (int i = 0; i < (int)_servers->size(); i++)
	{
		std::vector<Server> &temp = *_servers;
		Server &tempserver = temp[i];
		std::cout << "!! server getSocketListen() size is " << temp[i].getSocketListen().size() << std::endl;

		std::vector<SocketListen> tempsocketlist = tempserver.getSocketListen();
		std::cout << "getSocketListen() size is " << tempsocketlist.size() << std::endl;

		for (int i = 0; i < (int)tempsocketlist.size(); i++)
		{
			_listListeningSocketInt.push_back(tempsocketlist[i].getNumSocket());
		}
	}
}

KqueueLoop::~KqueueLoop()
{

}

KqueueLoop& KqueueLoop::operator=(const KqueueLoop &source)	
{
	if (this != &source)
	{
		_kq_main = source._kq_main;
		// _kev_catch[TOTAL_KEV] = source._kev_catch[TOTAL_KEV]; // can't copy
		_n_ev = source._n_ev;
		_listListeningSocketInt = source._listListeningSocketInt;
		_servers = source._servers;
	}
	return (*this);
}

KqueueLoop::KqueueLoop(const KqueueLoop &source)
{
	*this = source;
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


int KqueueLoop::startLoop()
{
	std::cout << "Kqueue start" << std::endl;
	while(1)
	{
		_n_ev = kevent(_kq_main, NULL, 0, _kev_catch, TOTAL_KEV, NULL);
		for (int i = 0; i < _n_ev; i++)
		{
			try
			{
				// check if the socket is closed by client (or error happens on the socket)
				if (_kev_catch[i].flags == EV_ERROR)
				{
					std::cout << "error on a socket" << _kev_catch[i].ident << std::endl;
					delete ((SocketConnect *)_kev_catch[i].udata);
					close(_kev_catch[i].ident);
					continue;
				}
				std::cout << "_kev_catch[i].ident is " << _kev_catch[i].ident << std::endl;

				int if_socket_listen = checkListeningSocket(_kev_catch[i].ident);
				if (if_socket_listen > 2)
				{
					// make new connection socket, connect the new socket to udata of kevent
					std::cout << "[Event on listenning socket] " << if_socket_listen << std::endl;
					_kev_catch[i].udata = new SocketConnect((int)_kev_catch[i].ident, _kq_main, _servers);
					if (_kev_catch[i].udata == NULL)
						throw std::invalid_argument("error on accepting new socket");
				}
				else // the socket is a connection socket
				{
					std::cout << "[!!!] " << std::endl;

					SocketConnect	*currentsocket = static_cast<SocketConnect *>(_kev_catch[i].udata);
					if (_kev_catch[i].filter == EVFILT_READ) // check if the socket is to read
					{
						// read and store data, make response data(not added yet), change kevent filter and set this to kev_catch
						std::cout << "[Event on connection socket(EVFILT_READ)] " << _kev_catch[i].ident << std::endl;
						currentsocket->setRequest(_servers);
						EV_SET(&_kev_catch[i], _kev_catch[i].ident, EVFILT_READ, EV_DELETE, 0, 0, _kev_catch[i].udata);
						EV_SET(&_kev_catch[i], _kev_catch[i].ident, EVFILT_WRITE, EV_ADD, 0, 0, _kev_catch[i].udata);
						kevent(_kq_main, &_kev_catch[i], 1, NULL, 0, NULL);
					}
					else if (_kev_catch[i].filter == EVFILT_WRITE) // check if the socket is to write
					{
						std::cout << "Event on connection socket(EVFILT_WRITE) " << currentsocket->getSocketConnect() << std::endl;
						if (currentsocket->getErrorNum() != 0)
						{
							// if _error is set, send error file
							std::cout << "send error! " << currentsocket->getErrorNum() << std::endl;
						}
						else 
						{
							// send response data, clean and close socket
							std::cout << "send response! " << std::endl;
							currentsocket->sendResponse();
						}
						delete (currentsocket);
						close(_kev_catch[i].ident);
					}
				}
			}
			catch (std::exception &e) // we must leave the error without using exit (otherwise the server stops for one error on a stream)
			{
				std::cout << e.what() << std::endl;
			}
		}
	}
	return (0);
}

// getter

int	KqueueLoop::getKqMain()
{
	return (_kq_main);
}


// exception
KqueueLoop::ERR_KqueueLoop::ERR_KqueueLoop():_error_msg("KqueueLoop setting failed"){}
KqueueLoop::ERR_KqueueLoop::ERR_KqueueLoop(const char *error_msg):_error_msg(error_msg){}

const char	*KqueueLoop::ERR_KqueueLoop::what() const _NOEXCEPT
{
    return (_error_msg);
}
