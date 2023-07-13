//
// Request
// Request class stores client's request information
//

#include "../../include/Request.hpp"
#include "../../include/macro.hpp"
#include "../../include/util.hpp"


Request::Request(): _contentLength(-1), _serverLocation(NULL)
{
	std::cout << "request made" << std::endl;
}

Request::~Request()
{

}

Request& Request::operator=(const Request &source)
{
	if (this != &source)
	{
		_requestMethod = source._requestMethod;
		_requestLocation = source._requestLocation;
		_requestHost = source._requestHost;
		_requestPort = source._requestPort;
		_requestHeaderOthers = source._requestHeaderOthers;
		_contentLength = source._contentLength;
		_contentBody = source._contentBody;
		_serverLocation = source._serverLocation;
	}
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

std::string		Request::getRequestPort()
{
	return (_requestPort);
}

int	Request::getContentLength()
{
	return (_contentLength);
}

void	Request::displayHeaderOthers()
{
	std::map<std::string, std::string>::iterator it;
	for (it =_requestHeaderOthers.begin(); it!=_requestHeaderOthers.end(); it++)
	{
		 std::cout << it->first << " : " << it->second << '\n';
	}
}

void	Request::displayHeaderAll()
{
	std::cout << "Method is " << getRequestMethod() << " Location is " << getRequestLocation() << std::endl;
	std::cout << "Host is " << getRequestHost() << " Port is " << getRequestPort()<< std::endl;
	std::cout << "Content length is " << getContentLength() << std::endl;
	displayHeaderOthers();
}

void	Request::setLabelContent(std::pair<std::string, std::string> datapair)
{
	_requestHeaderOthers.insert(datapair);
}

void	Request::setHostPortLength()
{
	std::map<std::string, std::string>::iterator it;

	it = _requestHeaderOthers.find("Host");
	if (it != _requestHeaderOthers.end())
	{
		std::string	hostport = it->second;
		_requestHost = splitString(&hostport, ":");
		_requestPort = removeWhitespace(hostport);
	}
		
	it = _requestHeaderOthers.find("Content-Length");
	if (it != _requestHeaderOthers.end())
	{
		_contentLength = stoi(it->second);
	}
}

bool	Request::checkMethod(std::vector<Server> list_Servers)
{
	try
	{
		Server	request_server = findServer(_requestHost, list_Servers);
		std::string request_servername = request_server.getServerName();
		std::cout << "This server is "  << request_servername << std::endl;
		DirSettings request_setting = findDirSetting(*this, request_server);
		if (!checkMethodAllowed(_requestMethod, request_setting))
		{
			std::cout << "This method is not allowd " << _requestMethod << std::endl;
			return (0);
		}
		return (1);
	}
	catch(std::exception &e)
	{
		return (0);
	}
}

