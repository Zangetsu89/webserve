//
// Socket for Connection
//

// #include "../../include/Request.hpp"

#include "../../include/SocketConnect.hpp"
#include "../../include/Request.hpp"
#include "../../include/Response.hpp"

SocketConnect::SocketConnect():_numSocket(0), _errorNum(0), _statusNum(0){}

SocketConnect::SocketConnect(int socket, int kq, std::vector<Server> *servers): _servers(servers), _errorNum(0), _statusNum(0)
{
	_numSocket = accept(socket, (struct sockaddr *) &_clientSockaddr, &_clientSockaddrLen);
	if (_numSocket < 3)
	{
		std::cout << "accept failed. _numSocket is " << _numSocket << std::endl;
		throw ERR_SocketConnect("accept new request failed");
	}
	_clientSockaddrLen = sizeof(_clientSockaddr);
	_timeout.tv_sec = 10; // set 10 second to max waiting time for data transfer 
	_timeout.tv_usec = 0;
	if (setsockopt(_numSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&_timeout, sizeof(_timeout)) < 0)
		throw ERR_SocketConnect("setsockopt for timeout failed");
	if (fcntl(_numSocket, F_SETFL, O_NONBLOCK) < 0)
		throw ERR_SocketConnect("fcntl failed");
	EV_SET(&_clientKevent, _numSocket, EVFILT_READ, EV_ENABLE | EV_ADD, 0, 0, this);
	if (kevent(kq, &_clientKevent, 1, NULL, 0, NULL) < 0)
		throw ERR_SocketConnect("kevent for socket failed");
}

SocketConnect::~SocketConnect()
{

}

SocketConnect& SocketConnect::operator=(const SocketConnect &source)
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
		_redirectURL = source._redirectURL;
		_timeout = source._timeout;
	}
	return (*this);
}

SocketConnect::SocketConnect(const SocketConnect &source)
{
	*this = source;
}

int SocketConnect::getNumSocket()
{
    return (_numSocket);
}

// getter

int	SocketConnect::getSocketConnect()
{
	return (_numSocket);
}

Request		*SocketConnect::getClientRequest()
{
	return (&_clientRequest);
}

Response		*SocketConnect::getClientResponse()
{
	return (&_clientResponse);
}

int	SocketConnect::getErrorNum()
{
	return (_errorNum);
}

int	SocketConnect::getStatusNum()
{
	return (_statusNum);
}


// setter and others

int SocketConnect::readRequest()
{
	char	buff[BUFFSIZE];
	int		bytesRead = 1;

	while (bytesRead > 0)
	{
		bytesRead = recv(_numSocket, buff, BUFFSIZE, 0);
		if (bytesRead == 0)
			break ;
		for (int i = 0; i < bytesRead; i++)
			_clientRequest.addDataR(buff[i]);
	}
	if (_clientRequest.getSizeR() == 0)
		throw std::invalid_argument("Request is empty");
	return (0);
}

void	SocketConnect::setRequest(std::vector<Server> *list_server)
{
	_clientRequest.setRequest(list_server, this);
}

void	SocketConnect::setError(int err)
{
	_errorNum = err;
}

void	SocketConnect::setStatus(int status)
{
	_statusNum = status;
}

void	SocketConnect::setRedirect(std::string url)
{
	_redirectURL = url;
}

std::string	SocketConnect::getRedirectURL()
{
    return (_redirectURL);
}

// exception
SocketConnect::ERR_SocketConnect::ERR_SocketConnect():_error_msg("SocketConnect setting failed"){}
SocketConnect::ERR_SocketConnect::ERR_SocketConnect(const char *error_msg):_error_msg(error_msg){}

const char	*SocketConnect::ERR_SocketConnect::what() const _NOEXCEPT
{
	return (_error_msg);
}
