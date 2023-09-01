//
// Socket for Connection
//

#include "../../include/Request.hpp"
#include "../../include/SocketConnect.hpp"

SocketConnect::SocketConnect(int socket, int kq, std::vector<Server> *servers): _servers(servers)
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
		// _clientResponse = source._clientResponse;
		_errorNum = source._errorNum;
		_redirectURL = source._redirectURL;
		// _errorInfo = source._errorInfo;
		_timeout = source._timeout;
	}
	return (*this);
}

SocketConnect::SocketConnect(const SocketConnect &source)
{
	*this = source;
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

int	SocketConnect::getErrorNum()
{
	return (_errorNum);
}


// setter and others

int SocketConnect::setRequest(std::vector<Server> *list_server)
{
	_errorNum = _clientRequest.setRequest(list_server, this);
	if (_errorNum != 0)
		return (_errorNum);
	return (0);
}

void	SocketConnect::setError(int err)
{
	_errorNum = err;
}

void	SocketConnect::setRedirect(std::string url)
{
	_redirectURL = url;
}


// writing function : not done yet...
int SocketConnect::sendResponse()
{
	// dummy response, if it is not redirect, send dummy 403 error
	// const char *dummydata = "HTTP/1.1 403 Forbidden\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n<!DOCTYPE html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"><title>Error 403</title><link href=\"css.css\" rel=\"stylesheet\"></head><body>Error 403</body></html>";
	if (_redirectURL != "")
	{
		const char *dummydata = "HTTP/1.1 302 Found\r\nLocation: ";
		const char *redirecturl = _redirectURL.c_str();
		write(_numSocket, dummydata, strlen(dummydata)); // dummy response
		write(_numSocket, redirecturl, strlen(redirecturl)); // dummy response
	}
	else
	{
		const char *dummydata = "HTTP/1.1 403 Forbidden\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n<!DOCTYPE html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"><title>Error 403</title><link href=\"css.css\" rel=\"stylesheet\"></head><body>Error 403</body></html>";
		write(_numSocket, dummydata, strlen(dummydata)); // dummy response
	}

	

	return (0);
}

// exception
SocketConnect::ERR_SocketConnect::ERR_SocketConnect():_error_msg("SocketConnect setting failed"){}
SocketConnect::ERR_SocketConnect::ERR_SocketConnect(const char *error_msg):_error_msg(error_msg){}

const char	*SocketConnect::ERR_SocketConnect::what() const _NOEXCEPT
{
	return (_error_msg);
}
