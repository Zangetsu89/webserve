#include "../include/RequestHeader.hpp"
#include "../include/Request.hpp"
#include "../include/util.hpp"
#include "../include/Server.hpp"

RequestHeader::RequestHeader()
{
}

RequestHeader::~RequestHeader()
{
}

RequestHeader &RequestHeader::operator=(const RequestHeader &source)
{
	if (this != &source)
	{
		_requestHeaderMethod = source._requestHeaderMethod;
		_requestHeaderLocation = source._requestHeaderLocation;
		_requestHeaderLocationParameter = source._requestHeaderLocationParameter;
		_requestHeaderHTTP_Protocol = source._requestHeaderHTTP_Protocol;
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


std::string RequestHeader::getRequestMethod() const
{
	return (_requestHeaderMethod);
}

std::string RequestHeader::getRequestLocation() const
{
	return (_requestHeaderLocation);
}

std::string RequestHeader::getRequestLocationParametor() const
{
	return (_requestHeaderLocationParameter);
}

std::string RequestHeader::getHTTPProtocol() const
{
	return (_requestHeaderHTTP_Protocol);
}

std::string RequestHeader::getRequestHost() const
{
	return (_requestHeaderHost);
}

std::string RequestHeader::getRequestPort() const
{
	return (_requestHeaderPort);
}

std::map<std::string, std::string> *RequestHeader::getHeaderOthers()
{
	return (&_requestHeaderOthers);
}

void RequestHeader::displayHeaderOthers()
{
	std::map<std::string, std::string>::iterator it;
	for (it = _requestHeaderOthers.begin(); it != _requestHeaderOthers.end(); it++)
	{
		std::cout << it->first << " : " << it->second << '\n';
	}
}

void RequestHeader::displayHeaderAll()
{
	std::cout << std::endl
			  << "RequestHeader method is " << _requestHeaderMethod << std::endl;
	std::cout << "RequestHeader location is " << _requestHeaderLocation << std::endl;
	std::cout << "RequestHeader location parametor is " << _requestHeaderLocationParameter << std::endl;
	std::cout << "RequestHeader HTTPprotocol is " << _requestHeaderHTTP_Protocol << std::endl;
	std::cout << "RequestHeader host is " << _requestHeaderHost << ", port is " << _requestHeaderPort << std::endl;
	std::cout << std::endl;
}

void RequestHeader::checkLocationParametor()
{
	std::string locationtemp = _requestHeaderLocation;
	std::string slicepart;

	slicepart = splitString(&locationtemp, "?");
	_requestHeaderLocation = slicepart;
	_requestHeaderLocationParameter = locationtemp;

}

int RequestHeader::setMethodLocationProtocol(std::vector<char> *dataR)
{
	std::string str_read = vectorToString(dataR);
	std::string slicepart;
	int	err = 0;

	slicepart = splitString(&str_read, " ");
	_requestHeaderMethod = slicepart;
	if (_requestHeaderMethod != "GET" && _requestHeaderMethod != "POST" && _requestHeaderMethod != "DELETE")
		err = 400;
	slicepart = splitString(&str_read, " ");
	_requestHeaderLocation = slicepart;
	checkLocationParametor();
	slicepart = splitString(&str_read, "\r\n");
	_requestHeaderHTTP_Protocol = slicepart;
	if (_requestHeaderHTTP_Protocol != "HTTP/1.1")
		err = 400;
	return (err);
}

int RequestHeader::setHeaderOthers(std::vector<char> *dataR)
{
	std::string str_read = vectorToString(dataR);
	std::string slicepart;

	splitString(&str_read, "\r\n"); // delete the first line (Method part)

	while (str_read.size())
	{
		size_t pos;
		slicepart = splitString(&str_read, "\r\n");
		pos = slicepart.find(": ");
		if (pos != std::string::npos)
		{
			std::pair<std::string, std::string> datapair = getLabelItem(&slicepart, ": ");
			_requestHeaderOthers.insert(datapair);
		}

	}
	return (0);
}

int RequestHeader::setHostPort()
{
	std::map<std::string, std::string>::iterator it;
	size_t it_str;

	it = _requestHeaderOthers.find("Host");
	if (it == _requestHeaderOthers.end())
		return (400);
	std::string hostPort = it->second;
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
