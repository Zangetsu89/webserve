#include "../include/SocketConnect.hpp"
#include "../include/KqueueLoop.hpp"
#include "../include/Request.hpp"
#include "../include/Response.hpp"
#include "../include/status.hpp"

SocketConnect::SocketConnect() : _numSocket(0), _kqueueNum(0) {}

SocketConnect::SocketConnect(int socket, int kq, std::vector<Server> *servers, std::vector<SocketConnect *> *list) 
{
	_servers = servers;
	_kqueueNum = kq;
	_socketsList = list;
	_numSocket = accept(socket, (struct sockaddr *)&_socketAddr, &_socketAddrLen);
	if (_numSocket < 3)
	{
		std::cout << "accept failed. _numSocket is " << _numSocket << std::endl;
		throw ERR_SocketConnect("accept new request failed");
	}
	EV_SET(&_socketKevent, _numSocket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
	if (kevent(kq, &_socketKevent, 1, NULL, 0, NULL) < 0)
		throw ERR_SocketConnect("kevent for socket failed");
}

SocketConnect::~SocketConnect()
{
}

SocketConnect &SocketConnect::operator=(const SocketConnect &source)
{
	if (this != &source)
	{
		_numSocket = source._numSocket;
		_kqueueNum = source._kqueueNum;
		_socketAddr = source._socketAddr;
		_socketAddrLen = source._socketAddrLen;
		_socketKevent = source._socketKevent;
		_servers = source._servers;
		_socketRequest = source._socketRequest;
		_socketResponse = source._socketResponse;
		_socketCgiHandler = source._socketCgiHandler;
	}
	return (*this);
}

SocketConnect::SocketConnect(const SocketConnect &source)
{
	*this = source;
}

int SocketConnect::getNumSocket() const
{
	return (_numSocket);
}


int SocketConnect::getKqueueNum() const
{
	return (_kqueueNum);
}

struct kevent	*SocketConnect::getSocketKevent()
{
	return (&_socketKevent);
}


Request *SocketConnect::getSocketRequest()
{
	return (&_socketRequest);
}

Response *SocketConnect::getSocketResponse()
{
	return (&_socketResponse);
}

CgiHandler	*SocketConnect::getsocketCgiHandler()
{
	return (&_socketCgiHandler);
}

void	SocketConnect::setKevent_READ()
{
	EV_SET(&_socketKevent, _socketKevent.ident, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
	kevent(_kqueueNum, &_socketKevent, 1, NULL, 0, NULL);
}

void	SocketConnect::setKevent_WRITE()
{
	EV_SET(&_socketKevent, _socketKevent.ident, EVFILT_READ, EV_DELETE, 0, 0, 0);
	EV_SET(&_socketKevent, _socketKevent.ident, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, 0);
	kevent(_kqueueNum, &_socketKevent, 1, NULL, 0, NULL);
}

int SocketConnect::readRequest()
{
	char buff[BUFFSIZE];
	int bytesRead = 1;

	bytesRead = recv(_numSocket, buff, BUFFSIZE, 0);
	if (bytesRead < 0)
		throw KqueueLoop::Exception_CloseSocket("recv failed");
	for (int i = 0; i < bytesRead; i++)
		_socketRequest.addDataR(buff[i]);

	return (bytesRead);
}

int SocketConnect::readResponseFile()
{
	char buff[BUFFSIZE];
	int bytesRead = 1;
	int	fd = getSocketResponse()->getResponseFD();

	bytesRead = read(fd, buff, BUFFSIZE);
	if (bytesRead < 0)
		throw KqueueLoop::Exception_CloseSocket("read response file failed");
	for (int i = 0; i < bytesRead; i++)
		_socketResponse.addCtoResponseBody(buff[i]);

	return (bytesRead);
}

void SocketConnect::setRequest(std::vector<Server> *list_server)
{
	_socketRequest.setRequest(list_server, this);
}

bool SocketConnect::doRedirect(std::vector<SocketConnect *> registerdSockets, int where)
{
	if (getSocketRequest()->getRequestDirSettings()->getRedirect().second.empty())
		return (0);
	else
	{
		int redirect_status = getSocketRequest()->getRequestDirSettings()->getRedirect().first;
		std::string redirect_url = getSocketRequest()->getRequestDirSettings()->getRedirect().second;
		std::string redirect_status_str = std::to_string(redirect_status);
		std::string redirect_message = set_Status(redirect_status);
		std::cout << "Redirect is set " << redirect_url << std::endl;

		const char *redirect_res_1 = "HTTP/1.1 ";
		const char *redirect_res_2 = redirect_status_str.c_str();
		const char *redirect_res_3 = redirect_message .c_str();
		const char *redirect_res_4 = "\r\nLocation: ";
		const char *redirect_res_5 = redirect_url.c_str();

		registerdSockets.erase(registerdSockets.begin() + where);
		write(getNumSocket(), redirect_res_1, strlen(redirect_res_1));
		write(getNumSocket(), redirect_res_2, strlen(redirect_res_2));
		write(getNumSocket(), redirect_res_3, strlen(redirect_res_3));
		write(getNumSocket(), redirect_res_4, strlen(redirect_res_4));
		write(getNumSocket(), redirect_res_5, strlen(redirect_res_5));
	}
	return (1);
}
// exception
SocketConnect::ERR_SocketConnect::ERR_SocketConnect() : _error_msg("SocketConnect setting failed") {}
SocketConnect::ERR_SocketConnect::ERR_SocketConnect(const char *error_msg) : _error_msg(error_msg) {}

const char *SocketConnect::ERR_SocketConnect::what() const noexcept
{
	return (_error_msg);
}
