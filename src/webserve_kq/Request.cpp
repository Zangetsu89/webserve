//
// Request
// Request class stores client's request information
//

#include "../../include/Request.hpp"
#include "../../include/macro.hpp"
#include "../../include/util.hpp"


Request::Request(): _contentLength(-1), _requestServer(NULL), _requestDirSetting(NULL)
{
	
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
		_requestFile = source._requestFile;
		_requestLocation = source._requestLocation;
		_requestHost = source._requestHost;
		_requestPort = source._requestPort;
		_requestHeaderOthers = source._requestHeaderOthers;
		_contentLength = source._contentLength;
		_contentBody = source._contentBody;
		_requestServer = source._requestServer;
		_requestDirSetting = source._requestDirSetting;
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
	std::cout << "Request method is " << getRequestMethod() << ", Location is " << getRequestLocation() << std::endl;
	std::cout << "Request host is " << getRequestHost() << ", port is " << getRequestPort()<< std::endl;
	std::cout << "Request content length is " << getContentLength() << std::endl;
	// displayHeaderOthers();	// to display all of request headers
	std::cout << std::endl;
}

void	Request::setHeaderOthers(std::pair<std::string, std::string> datapair)
{
	_requestHeaderOthers.insert(datapair);
}

void	Request::setFile()
{
	if (_requestLocation.back() == '/')
	{
		_requestLocation.pop_back();
		_requestFile = _requestDirSetting->getindexFile()[0];
		return ;
	}
	size_t			pos_slice;
	std::string		newstring;
	pos_slice = _requestLocation.rfind('/');
	newstring = _requestLocation.substr(pos_slice + 1, _requestLocation.size());
	_requestFile = newstring;
	_requestLocation.erase(pos_slice, _requestLocation.size());
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

bool	Request::checkMethod()
{
	try
	{
		if (!checkMethodAllowed(_requestMethod, _requestDirSetting))
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

