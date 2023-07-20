//
// Request
// Request class stores client's request information
//

#include "../../include/RequestHeader.hpp"
#include "../../include/Request.hpp"
#include "../../include/SocketConnect.hpp"
#include "../../include/util.hpp"
#include <sys/stat.h>

Request::Request()
{
	
}

Request::~Request()
{

}

Request& Request::operator=(const Request &source)
{
	if (this != &source)
	{
		_requestHeader = source._requestHeader;
		_requestFilePath = source._requestFilePath;
		_requestBodyLength = source._requestBodyLength;
		_requestBody = source._requestBody;
		_requestSocket = source._requestSocket;
		_servers = source._servers;
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

std::string	Request::getRequestFilePath()
{
	return (_requestFilePath);
}

RequestHeader	*Request::getRequestHeader()
{
	return (&_requestHeader);
}

void Request::printDataR()
{
	std::cout << std::endl << "request data start ------------- " << std::endl;
	for (int i = 0; i < (int)_dataR.size(); i++)
		std::cout << _dataR[i];
	std::cout << "request data end --------------- " << std::endl << std::endl;
}

void Request::printSizeR()
{
	std::cout << "data size is " << _sizeR << std::endl;
}

int Request::setRequest(std::vector<Server> *list_server, SocketConnect *socket)
{
	_servers = list_server;
	_requestSocket = socket;
	try
	{
		readRequest();
		setRequestHeader();
		setRequestBodyLength();
		setRequestBody();
		findServer();
		findDirSetting();
		checkProtocol();
		findResponseFile();
	}
	catch (ERR_Request& e)
	{
		std::cout << "err is " << e._error_num << '\n';
		return (e._error_num);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}

	// // to print the header parser result
	// _requestHeader.displayHeaderOthers();
	// _requestHeader.displayHeaderAll();
	return (0);
}

int Request::readRequest()
{
	char	buff[BUFF];
	int		r = 1;
	// maybe we have to set the time out
	while (r > 0)
	{
		r = read(_requestSocket->getSocketConnect(), buff, BUFF);
		if (r <= 0)
			break ;
		for (int i = 0; i < r; i++)
		{
			_dataR.push_back(buff[i]);
			_sizeR++;
		}
	}
	// to print the request
	printDataR();
	// printSizeR();
	return (0);
}

int	Request::setRequestHeader()
{
	int res = _requestHeader.setMethodLocationProtocol(&_dataR);
	if (res != 0)
		throw ERR_Request("The first line of request is wront", res);
	res = _requestHeader.setHeaderOthers(&_dataR);
	if (res != 0)
		throw ERR_Request("Header information is wrong", res);
	return (_requestHeader.setHostPort());
}

int Request::setRequestBodyLength()
{
	std::map<std::string, std::string>::iterator it;

	it = _requestHeader.getHeaderOthers()->find("Content-Length");
	if (it != _requestHeader.getHeaderOthers()->end())
		_requestBodyLength = stoi(it->second);
	return (0);
}

int Request::setRequestBody()
{
	std::string	str_read = toString(&_dataR);
	std::string	slicepart;

	try
	{
		slicepart = splitString(&str_read, "\n\n");
		_requestBody = str_read;
	}
	catch(const std::exception& e)
	{
		// this case, there is no request body (only header)
	}
	return (0);
}

int	Request::findServer()
{
	std::vector<Server>::iterator	it;
	for (it = _servers->begin(); it != _servers->end(); it++)
	{
		if (_requestHeader.getRequestHost() == it->getServerName())
		{
			_requestServer = &(*it);
			return (0);
		}		
	}
	throw ERR_Request("server(host) is not found", 400);
}

int	Request::findDirSetting()
{
	std::string					requestLocation;
	std::vector<DirSettings> 	*list_dirsetting = _requestServer->getOptDirSettings();

	requestLocation = _requestHeader.getRequestLocation();
	if (requestLocation.back() == '/')
		requestLocation.pop_back();
	
	for (; requestLocation != ""; deleteStringEnd(&requestLocation, "/"))
	{
		for (std::vector<DirSettings>::iterator it = list_dirsetting->begin(); it != list_dirsetting->end(); it++)
		{
			if (requestLocation == it->getLocation())
			{
				_requestDirSetting = &(*it);
				return (0);
			}
		}
	}
	_requestDirSetting = _requestServer->getRootDirSettings();
	// std::cout << "requestDir is gotten " << _requestServer->getRootDirSettings()->getLocation() << std::endl;
	return (0);
}

int Request::checkMethod()
{
	std::vector<std::string> allowedmethod = _requestDirSetting->getMethods();
	for (std::vector<std::string>::iterator it = allowedmethod.begin(); it != allowedmethod.end(); it++)
	{
		if (_requestHeader.getRequestMethod() == *it)
			return (0);
	}
	throw ERR_Request("Method not allowed", 405);
}

int Request::checkProtocol()
{
	if (_requestHeader.getHTTPProtocol() != "HTTP/1.1")
		throw ERR_Request("Protocol not allowed", 400);
	return (0);
}

int Request::findResponseFile()
{
	std::string filepath = _requestHeader.getRequestLocation();
	// std::cout << "filepath is " << filepath << std::endl;
	if (filepath.back() == '/')
	{
		filepath.pop_back();
		_requestFilePath = _requestServer->getRootDir() + _requestDirSetting->getindexFile()[0];
		return (0);
	}
	_requestFilePath = _requestServer->getRootDir() + filepath;
	// std::cout << "_requestFilePath is " << _requestFilePath << std::endl;
	struct stat	status;
	if (stat(_requestFilePath.c_str(), &status) != 0)
		throw ERR_Request("file not found", 404);
	return (0);
}

// exception
Request::ERR_Request::ERR_Request() : _error_msg("Request setting failed"), _error_num(0) {}
Request::ERR_Request::ERR_Request(const char *error_msg, int err) : _error_msg(error_msg), _error_num(err)
{
	
}

const char *Request::ERR_Request::what() const _NOEXCEPT
{
	return (_error_msg);
}