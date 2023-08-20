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

void	SocketConnect::setError()
{
    std::cout << "TEST in setError " << _clientRequest.getRequestErrorNum() << std::endl;
	_errorNum = _clientRequest.getRequestErrorNum();
}

void	SocketConnect::setRedirect(std::string url)
{
	_redirectURL = url;
}

std::string	SocketConnect::getRedirectURL()
{
    return (_redirectURL);
}


// writing function : not done yet...
int SocketConnect::sendResponse()
{
    try {
        // read from _clientRequest._requestFilePath

        std::ifstream file(_clientRequest._requestFilePath.c_str());
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string contents(buffer.str());

        std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n" + contents;
        write(_numSocket, response.c_str(), response.length());
    } catch (std::exception &e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
    // std::cout << "TEST in sendResponse" << std::endl;
	// dammy response, simular with 403 error
//    const char *dammydata = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n"
//                            "<!DOCTYPE html><html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">"
//                            "<title>Error 404</title><link href=\"css.css\" rel=\"stylesheet\"></head><body>"
//                            "<h1>Error 404</h1><p>The requested page could not be found.</p></body></html>";
//	const char *dammydata = "HTTP/1.1 403 Forbidden\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n<!DOCTYPE html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"><title>Error 403</title><link href=\"css.css\" rel=\"stylesheet\"></head><body>Error 403</body></html>";
//	write(_numSocket, dammydata, strlen(dammydata)); // dammy response
//    std::cout << "TESTTTTTT" << _numSocket << std::endl;
	// dummy response, if it is not redirect, send dummy 403 error
	// const char *dummydata = "HTTP/1.1 403 Forbidden\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n<!DOCTYPE html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"><title>Error 403</title><link href=\"css.css\" rel=\"stylesheet\"></head><body>Error 403</body></html>";

    // what we pulled from main
//	if (_redirectURL != "")
//	{
//		const char *dummydata = "HTTP/1.1 302 Found\r\nLocation: ";
//		const char *redirecturl = _redirectURL.c_str();
//		write(_numSocket, dummydata, strlen(dummydata)); // dummy response
//		write(_numSocket, redirecturl, strlen(redirecturl)); // dummy response
//	}
//	else
//	{
//		const char *dummydata = "HTTP/1.1 403 Forbidden\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n<!DOCTYPE html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"><title>Error 403</title><link href=\"css.css\" rel=\"stylesheet\"></head><body>Error 403</body></html>";
//		write(_numSocket, dummydata, strlen(dummydata)); // dummy response
//	}


	return (0);
}

// exception
SocketConnect::ERR_SocketConnect::ERR_SocketConnect():_error_msg("SocketConnect setting failed"){}
SocketConnect::ERR_SocketConnect::ERR_SocketConnect(const char *error_msg):_error_msg(error_msg){}

const char	*SocketConnect::ERR_SocketConnect::what() const _NOEXCEPT
{
	return (_error_msg);
}

bool SocketConnect::isCGI() const {
    // Check if the request location matches the CGI location
    // You might need to modify this logic based on your URL parsing logic
    if (_clientRequest._requestFilePath.find("/cgi-bin/") == 0) {
        return true;
    }
    return false;
}
