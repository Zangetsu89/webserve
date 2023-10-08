#include "../include/KqueueLoop.hpp"
#include "../include/CgiHandler.hpp"
#include "../include/SocketConnect.hpp"
#include "../include/SocketListen.hpp"
#include "../include/Response.hpp"
#include "../include/macro.hpp"

KqueueLoop::KqueueLoop(std::vector<Server> *servers, int kq) : _kq_main(kq), _n_ev(0), _servers(servers)
{
	for (int i = 0; i < TOTAL_KEV; i++)
		EV_SET(&_kev_catch[i], -1, EVFILT_READ, EV_DISABLE, 0, 0, NULL);
}

KqueueLoop::~KqueueLoop()
{
}

static int getEventIndexFromSocketsList(std::vector<SocketConnect *> registerdSockets, int eventFd)
{
	for (int i = 0; i < (int)registerdSockets.size(); i++)
	{
		if (registerdSockets[i]->getNumSocket() == eventFd)
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
	std::vector<SocketConnect *> registerdSockets;

	while (1)
	{
		_n_ev = kevent(_kq_main, NULL, 0, _kev_catch, TOTAL_KEV, NULL);
		for (int i = 0; i < _n_ev; i++)
		{
			try
			{
				if ( _kev_catch[i].flags & EV_ERROR)
					throw Exception_CloseSocket("error flag on a socket");				
				int monitored_socket = getEventIndexFromSocketsList(registerdSockets, (int)_kev_catch[i].ident);	
				// if it is new request -> create new socket
				if (monitored_socket < 0 && !_kev_catch[i].udata)
				{
					std::cout << std::endl << "[New Request] " << _kev_catch[i].ident<< std::endl;
					SocketConnect *newSocket = new SocketConnect((int)_kev_catch[i].ident, _kq_main, _servers, &registerdSockets);
					if (newSocket == NULL)
						throw Exception_CloseSocket("error on accepting new socket");
					registerdSockets.push_back(newSocket);
					continue;
				}
				else if (_kev_catch[i].filter == EVFILT_READ)
				{			
					// case for reading from request socket		
					if (monitored_socket >= 0)
					{
						SocketConnect *currentsocket = registerdSockets[monitored_socket];
						int r = currentsocket->readRequest();
						if (r == BUFFSIZE || currentsocket->getSocketRequest()->getSizeR() == 0)
						{
							currentsocket->setKevent_READ();
							continue;
						}
						currentsocket->setRequest(_servers);
						std::cout << std::endl << "[Request is set] " << _kev_catch[i].ident<< std::endl;
						// check if the request is chanked data
						if (currentsocket->getSocketRequest()->getRequestBodyLength() > (int)(currentsocket->getSocketRequest()->getRequestBody())->size())
						{
							currentsocket->setKevent_READ();
							continue;
						}
						std::cout << std::endl << "[Prepare response] " << _kev_catch[i].ident<< std::endl;
						currentsocket->getSocketResponse()->setResponseBody(currentsocket);
						EV_SET(&_kev_catch[i], _kev_catch[i].ident, EVFILT_READ, EV_DELETE, 0, 0, 0);
						kevent(_kq_main, &_kev_catch[i], 1, NULL, 0, NULL);
						continue;
					}
					// case for reading from a file or CGI
					Response	*resp = (Response *)_kev_catch[i].udata;
					if (resp->getResponseSocket()->getSocketRequest()->checkCGI()) // reading CGI executed data
					{
						char buff;
						int	readTotal = 1;
						while (readTotal > 0)
						{
							readTotal = read(_kev_catch[i].ident, &buff, 1);
							if (readTotal < 0)
								throw Exception_CloseSocket("error on reading CGI");
							resp->addCtoResponseBody(buff);
						}
					}
					else if (resp->readFile(_kev_catch[i].ident) == BUFFSIZE) // reading a file data
					{
						EV_SET(&_kev_catch[i], _kev_catch[i].ident, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, _kev_catch[i].udata);
						kevent(_kq_main, &_kev_catch[i], 1, NULL, 0, NULL);
						continue;
					}
					resp->setStatus(200);
					resp->makeResponseHeader();
					resp->makeResponseFullData();
					close(_kev_catch[i].ident);
					struct kevent *socket_kev = resp->getResponseSocket()->getSocketKevent();
					EV_SET(socket_kev, socket_kev->ident, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, 0);
					kevent(_kq_main, socket_kev, 1, NULL, 0, NULL);
					continue;
				}
				else if (_kev_catch[i].filter == EVFILT_WRITE)
				{
					if (monitored_socket >= 0) // send response to client
					{
						SocketConnect *currentsocket = registerdSockets[monitored_socket];
						if (currentsocket->getSocketResponse()->sendResponse(currentsocket->getNumSocket()))
						{
							EV_SET(&_kev_catch[i], _kev_catch[i].ident, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, 0);
							kevent(_kq_main, &_kev_catch[i], 1, NULL, 0, NULL);
							continue;
						}
						delete(registerdSockets[monitored_socket]);
						registerdSockets.erase(registerdSockets.begin() + monitored_socket);
						close(_kev_catch[i].ident);
						continue;
					}
					// send POST data to CGI child process 
					Response	*resp = (Response *)_kev_catch[i].udata;
					std::string *postdata = resp->getResponseSocket()->getSocketRequest()->getRequestBody();
					const char *charstr = postdata->c_str();
					EV_SET(resp->getResponseSocket()->getsocketCgiHandler()->getCgiExecKevent(), resp->getResponseCGIFD(), EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, _kev_catch[i].udata);
					kevent(_kq_main, resp->getResponseSocket()->getsocketCgiHandler()->getCgiExecKevent(), 1, NULL, 0, NULL);
					int sent = write(_kev_catch[i].ident, charstr , (int)postdata->size());
					if (sent < 0)
						throw Exception_CloseSocket("write failed");
					close(_kev_catch[i].ident);
					continue;
				}	
			}
			catch (std::exception &e) // leave the error without exit
			{
				close(_kev_catch[i].ident);
				std::cout << e.what() << std::endl;
				int num_socket_registered = getEventIndexFromSocketsList(registerdSockets, _kev_catch[i].ident);
				if (num_socket_registered >= 0)
				{
					delete (registerdSockets[num_socket_registered]);
					registerdSockets.erase(registerdSockets.begin() + num_socket_registered);
				}
			}
		}
	}
	return (0);
}

// exception
KqueueLoop::Exception_CloseSocket::Exception_CloseSocket() : _error_msg("KqueueLoop setting failed") {}
KqueueLoop::Exception_CloseSocket::Exception_CloseSocket(const char *error_msg) : _error_msg(error_msg) {}

const char *KqueueLoop::Exception_CloseSocket::what() const noexcept
{
	return (_error_msg);
}
