//
// Request
// Request class stores client's request information
//

#include "../../include/Request.hpp"
#include "../../include/macro.hpp"


Request::Request(): _contentLength(-1), _serverLocation(NULL)
{
	std::cout << "request made" << std::endl;
}

Request::~Request()
{

}

Request& Request::operator=(const Request &source)
{
	_requestMethod = source._requestMethod;
	_requestLocation = source._requestLocation;
	_requestHost = source._requestHost;
	_requestPort = source._requestPort;
	_requestBeforeContentLength = source._requestBeforeContentLength;
	_contentLength = source._contentLength;
	_contentBody = source._contentBody;
	_serverLocation = source._serverLocation;
	return (*this);
}

Request::Request(const Request &source)
{
	*this = source;
}

// getter

std::string	Request::getRequestMethod()
{
	return (_requestMethod);
}

std::string		Request::getRequestLocation()
{
	return (_requestLocation);
}

std::string		Request::getRequestHost()
{
	return (_requestHost);
}

int	Request::getContentLength()
{
	return (_contentLength);
}


void	Request::checkMethod(std::vector<Server> list_Servers)
{
	// in construction
	std::string	location_host = _requestLocation + _requestHost;
	(void)list_Servers;
}

