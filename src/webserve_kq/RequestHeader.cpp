//
// RequestHeader
//

#include "../../include/RequestHeader.hpp"
#include "../../include/util.hpp"
#include "../../include/Server.hpp"

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
		_requestHeaderMethod = source._requestHeaderMethod;
		_requestHeaderLocation = source._requestHeaderLocation;
		_requestHeaderLocationParametor = source._requestHeaderLocationParametor;
		_requestHeaderHTTPprotocol = source._requestHeaderHTTPprotocol;
		_requestHeaderHost = source._requestHeaderHost;
		_requestHeaderPort = source._requestHeaderPort;
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
	return (_requestHeaderMethod);
}

std::string		RequestHeader::getRequestLocation()
{
	return (_requestHeaderLocation);
}

std::string		RequestHeader::getRequestLocationParametor()
{
	return (_requestHeaderLocationParametor);
}

std::string RequestHeader::getHTTPProtocol()
{
	return (_requestHeaderHTTPprotocol);
}

std::string		RequestHeader::getRequestHost()
{
	return (_requestHeaderHost);
}

std::string		RequestHeader::getRequestPort()
{
	return (_requestHeaderPort);
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
	std::cout << std::endl << "RequestHeader method is " << _requestHeaderMethod << std::endl;
	std::cout << "RequestHeader location is " << _requestHeaderLocation << std::endl;
	std::cout << "RequestHeader location parametor is " << _requestHeaderLocationParametor<< std::endl;
	std::cout << "RequestHeader HTTPprotocol is " << _requestHeaderHTTPprotocol << std::endl;
	std::cout << "RequestHeader host is " << _requestHeaderHost << ", port is " << _requestHeaderPort << std::endl;
	std::cout << std::endl;
}

void	RequestHeader::checkLocationParametor()
{
	std::string locationtemp = _requestHeaderLocation;
	std::string slicepart;
	try
	{
		slicepart = splitString(&locationtemp, "?");
		std::cout << "slicepart is " << slicepart << std::endl;
		_requestHeaderLocation = slicepart;
		_requestHeaderLocationParametor = locationtemp;
		std::cout << "parametor is " << _requestHeaderLocationParametor << std::endl;
	}
	catch(const std::exception& e)
	{
	}
}

int	RequestHeader::setMethodLocationProtocol(std::vector<char> *dataR)
{
	std::string	str_read = vectorToString(dataR);
	std::string	slicepart;
	try
	{
		slicepart = splitString(&str_read, " ");
		_requestHeaderMethod = slicepart;
		if (_requestHeaderMethod != "GET" && _requestHeaderMethod != "POST" && _requestHeaderMethod != "DELETE")
			return (400); // bad request
		slicepart = splitString(&str_read, " ");
		_requestHeaderLocation = slicepart;
		if (_requestHeaderLocation[0] != '/') // request location must start by "/""
			return (400);
		checkLocationParametor();
		slicepart = splitString(&str_read, "\r\n");
		_requestHeaderHTTPprotocol = slicepart;
		if (_requestHeaderHTTPprotocol != "HTTP/1.1")
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
	std::string	str_read = vectorToString(dataR);
	std::string	slicepart;

	try
	{
		splitString(&str_read, "\r\n"); // delete the first line
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
			slicepart = splitString(&str_read, "\r\n");
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
	std::string	hostPort = it->second;
	it_str = hostPort.find(":");
	if (it_str == std::string::npos)
	{
		_requestHeaderHost = removeWhitespace(hostPort);
		_requestHeaderPort = "80";
	}
	else
	{
		_requestHeaderHost = splitString(&hostPort, ":");
		_requestHeaderPort = removeWhitespace(hostPort);
	}
	return (0);
}


