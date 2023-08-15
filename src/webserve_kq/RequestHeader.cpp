//
// RequestHeader
//

#include "../../include/RequestHeader.hpp"
#include "../../include/util.hpp"

RequestHeader::RequestHeader()
{
	
}

RequestHeader::~RequestHeader()
{
	
}

RequestHeader& RequestHeader::operator=(const RequestHeader &source)
{
	if (this != &source)
	{
		_requestMethod = source._requestMethod;
		_requestLocation = source._requestLocation;
		_requestLocation = source._requestLocation;
		_requestHost = source._requestHost;
		_requestPort = source._requestPort;
		_requestHeaderOthers = source._requestHeaderOthers;

		_requestSocket = source._requestSocket;
		_servers = source._servers;
	}
	return (*this);
}

RequestHeader::RequestHeader(const RequestHeader &source)
{
	*this = source;
}

// getter

std::string	RequestHeader::getRequestMethod()
{
	return (_requestMethod);
}

std::string		RequestHeader::getRequestLocation()
{
	return (_requestLocation);
}

std::string RequestHeader::getHTTPProtocol()
{
	return (_requestHTTPprotocol);
}

std::string		RequestHeader::getRequestHost()
{
	return (_requestHost);
}

std::string		RequestHeader::getRequestPort()
{
	return (_requestPort);
}

std::map<std::string, std::string> *RequestHeader::getHeaderOthers()
{
	return (&_requestHeaderOthers);
}

void	RequestHeader::displayHeaderOthers()
{
	std::map<std::string, std::string>::iterator it;
	for (it =_requestHeaderOthers.begin(); it!=_requestHeaderOthers.end(); it++)
	{
		 std::cout << it->first << " : " << it->second << '\n';
	}
}

void	RequestHeader::displayHeaderAll()
{
	std::cout << "RequestHeader method is " << _requestMethod << std::endl;
	std::cout << "RequestHeader location is " << _requestLocation << std::endl;
	std::cout << "RequestHeader host is " << _requestHost << ", port is " << _requestPort << std::endl;
	std::cout << std::endl;
}

int	RequestHeader::setMethodLocationProtocol(std::vector<char> *dataR)
{
	std::string	str_read = toString(dataR);
	std::string	slicepart;
	try
	{
		 slicepart = splitString(&str_read, " ");
		 _requestMethod = slicepart;
		 if (_requestMethod != "GET" && _requestMethod != "POST" && _requestMethod != "DELETE")
			 return (400); // bad request

		 slicepart = splitString(&str_read, " ");
		 _requestLocation = slicepart;
		 if (_requestLocation[0] != '/') // request location must start by "/""
			 return (400);

		 slicepart = splitString(&str_read, "\n");
		 _requestHTTPprotocol = slicepart;
		 if (_requestHTTPprotocol != "HTTP/1.1")
			 return (400);
	}
	catch(const std::exception& e)
	{
		std::cerr << "Bad request" << '\n';
		return (400);
	}
	return (0);
}

int	RequestHeader::setHeaderOthers(std::vector<char> *dataR)
{
	std::string	str_read = toString(dataR);
	std::string	slicepart;

	try
	{
		splitString(&str_read, "\n"); // delete the first line
	}
	catch(const std::exception& e)
	{
		std::cout << "no line" << std::endl;
		return (400); // if no line in the data, return bad request
	}
	// store other elements of the request to _requestHeaderOthers, and get content body
	while(str_read.size() != 0)
	{
		try
		{
			slicepart = splitString(&str_read, "\n");
			std::pair<std::string, std::string> datapair = getLabelItem(&slicepart, ": ");
			_requestHeaderOthers.insert(datapair);
		}
		catch(const std::exception& e) // this case, the line is empty new line or doesn't contain ": " 
		{
			break;
		}
	}
	return (0);
}

int	RequestHeader::setHostPort()
{
	std::map<std::string, std::string>::iterator it;
	size_t	it_str;

	it = _requestHeaderOthers.find("Host");
	if (it == _requestHeaderOthers.end())
		return (400);
	std::string	hostport = it->second;
	it_str = hostport.find(":");
	if (it_str == std::string::npos)
	{
		_requestHost = hostport;
		_requestPort = 80;
	}	
	else
	{
		_requestHost = splitString(&hostport, ":");
		_requestPort = removeWhitespace(hostport);
	}
	return (0);
}
