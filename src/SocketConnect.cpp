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

// exception
SocketConnect::ERR_SocketConnect::ERR_SocketConnect() : _error_msg("SocketConnect setting failed") {}
SocketConnect::ERR_SocketConnect::ERR_SocketConnect(const char *error_msg) : _error_msg(error_msg) {}

const char *SocketConnect::ERR_SocketConnect::what() const noexcept
{
	return (_error_msg);
}
