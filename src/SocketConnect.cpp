#include "../include/SocketConnect.hpp"
#include "../include/KqueueLoop.hpp"
#include "../include/Request.hpp"
#include "../include/Response.hpp"
#include "../include/status.hpp"

SocketConnect::SocketConnect() : _numSocket(0), _errorNum(0), _statusNum(0) {}

SocketConnect::SocketConnect(int socket, int kq, std::vector<Server> *servers) : _servers(servers), _errorNum(0), _statusNum(0)
{
	_numSocket = accept(socket, (struct sockaddr *)&_clientSockaddr, &_clientSockaddrLen);
	if (_numSocket < 3)
	{
		std::cout << "accept failed. _numSocket is " << _numSocket << std::endl;
		throw ERR_SocketConnect("accept new request failed");
	}
	_clientSockaddrLen = sizeof(_clientSockaddr);
	_timeout.tv_sec = 10; // set 10 second to max waiting time for data transfer
	_timeout.tv_usec = 0;
	if (fcntl(_numSocket, F_SETFL, O_NONBLOCK) < 0)
		throw ERR_SocketConnect("fcntl failed");
	if (setsockopt(_numSocket, SOL_SOCKET, SO_RCVTIMEO, (const char *)&_timeout, sizeof(_timeout)) < 0)
		throw ERR_SocketConnect("setsockopt for timeout failed");
	EV_SET(&_clientKevent, _numSocket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, this);
	if (kevent(kq, &_clientKevent, 1, NULL, 0, NULL) < 0)
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
		_clientSockaddr = source._clientSockaddr;
		_clientSockaddrLen = source._clientSockaddrLen;
		_clientKevent = source._clientKevent;
		_servers = source._servers;
		_clientRequest = source._clientRequest;
		_clientResponse = source._clientResponse;
		_errorNum = source._errorNum;
		_statusNum = source._statusNum;
		_timeout = source._timeout;
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


Request *SocketConnect::getClientRequest()
{
	return (&_clientRequest);
}

Response *SocketConnect::getClientResponse()
{
	return (&_clientResponse);
}

int SocketConnect::getErrorNum() const
{
	return (_errorNum);
}

int SocketConnect::getStatusNum() const
{
	return (_statusNum);
}

void SocketConnect::setError(int err)
{
	_errorNum = err;
}

void SocketConnect::setStatus(int status)
{
	_statusNum = status;
}

int SocketConnect::readRequest()
{
	char buff[BUFFSIZE];
	int bytesRead = 1;

	bytesRead = recv(_numSocket, buff, BUFFSIZE, 0);
	if (bytesRead < 0)
		throw KqueueLoop::Exception_CloseSocket("recv failed");
	for (int i = 0; i < bytesRead; i++)
		_clientRequest.addDataR(buff[i]);

	return (bytesRead);
}

void SocketConnect::setRequest(std::vector<Server> *list_server)
{
	_clientRequest.setRequest(list_server, this);
}

bool SocketConnect::doRedirect(std::vector<SocketConnect *> socketConnects, int where)
{
	if (getClientRequest()->getRequestDirSettings()->getRedirect().second.empty())
		return (0);
	else
	{
		int redirect_status = getClientRequest()->getRequestDirSettings()->getRedirect().first;
		std::string redirect_url = getClientRequest()->getRequestDirSettings()->getRedirect().second;
		std::string redirect_status_str = std::to_string(redirect_status);
		std::string redirect_message = set_Status(redirect_status);
		std::cout << "Redirect is set " << redirect_url << std::endl;

		const char *redirect_res_1 = "HTTP/1.1 ";
		const char *redirect_res_2 = redirect_status_str.c_str();
		const char *redirect_res_3 = redirect_message .c_str();
		const char *redirect_res_4 = "\r\nLocation: ";
		const char *redirect_res_5 = redirect_url.c_str();

		socketConnects.erase(socketConnects.begin() + where);
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

const char *SocketConnect::ERR_SocketConnect::what() const _NOEXCEPT
{
	return (_error_msg);
}
