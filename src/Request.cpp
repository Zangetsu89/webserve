#include "../include/Request.hpp"
#include "../include/SocketConnect.hpp"
#include "../include/util.hpp"

Request::Request() : _sizeR(0), _requestBodyLength(0), _requestShowList(0), _requestCGI(0)
{
}

Request::~Request()
{
}

Request &Request::operator=(const Request &source)
{
	if (this != &source)
	{
		_dataR = source._dataR;
		_sizeR = source._sizeR;
		_requestHeader = source._requestHeader;
		_requestContentType = source._requestContentType;
		_requestBodyLength = source._requestBodyLength;
		_requestBody = source._requestBody;
		_requestShowList = source._requestShowList;
		_requestCGI = source._requestCGI;

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

int Request::getSizeR() const
{
	return (_sizeR);
}

RequestHeader *Request::getRequestHeader()
{
	return (&_requestHeader);
}

std::string Request::getRequestContentType() const
{
	return (_requestContentType);
}

int Request::getRequestBodyLength() const
{
	return (_requestBodyLength);
}

std::string Request::getRequestBody() const
{
	return (_requestBody);
}

Server *Request::getRequestServer()
{
	return (_requestServer);
}

DirSettings *Request::getRequestDirSettings()
{
	return (_requestDirSetting);
}

void Request::printDataR()
{
	std::cout << std::endl
			  << "request data start ------------- " << std::endl;
	for (int i = 0; i < (int)_dataR.size(); i++)
		std::cout << _dataR[i];
	std::cout << "request data end --------------- " << std::endl
			  << std::endl;
}

void Request::printSizeR()
{
	std::cout << "data size is " << _sizeR << std::endl;
}

void Request::printRequestParsedData()
{
	std::cout << std::endl
			  << "Request class data start ------------- " << std::endl;
	_requestHeader.displayHeaderOthers();
	_requestHeader.displayHeaderAll();
	std::cout << "_requestContentType is " << _requestContentType << std::endl;
	std::cout << "_requestBodyLength is " << _requestBodyLength << std::endl;
	std::cout << "---_requestBody is " << _requestBody << std::endl;
	std::cout << "--- actual _requestBody size is " << _requestBody.size() << std::endl;
	std::cout << "Request class data end --------------- " << std::endl
			  << std::endl;
}

bool Request::getRequestShowList()
{
	return (_requestShowList);
}


void Request::addDataR(char c)
{
	_dataR.push_back(c);
	_sizeR++;
}


void Request::setRequest(std::vector<Server> *list_server, SocketConnect *socket)
{
	_servers = list_server;
	_requestSocket = socket;

	try
	{
		setRequestHeader();
		setRequestContentType();
		findServer();
		findDirSetting_checkCGI();
		checkRedirect();
		checkProtocol();
		setRequestBodyLength();
		setRequestBody();
		checkMethod();
	}
	catch (Exception_Request &e)
	{
		std::cout << "Exception in parsing Request data: " << e.what() << std::endl;
		_requestSocket->setError(e._error_num);
		_requestSocket->setStatus(e._status_num);
	}

	// print to check
	// printDataR();
	// printRequestParsedData();
}

int Request::setRequestHeader() // The first line of request must be correct. if not, send error response
{
	int res = _requestHeader.setMethodLocationProtocol(&_dataR);
	if (res != 0)
	{
		throw Exception_Request("The first line of request is wrong", res, 0);
	}
	res = _requestHeader.setHeaderOthers(&_dataR);
	if (res != 0)
	{
		throw Exception_Request("Header information is wrong", res, 0);
	}
	return (_requestHeader.setHostPort());
}

int Request::setRequestContentType() // Content-Type may not exist, depends on the request.

{
	std::map<std::string, std::string>::iterator it;

	it = _requestHeader.getHeaderOthers()->find("Content-Type");
	if (it != _requestHeader.getHeaderOthers()->end())
		_requestContentType = it->second;
	return (0);
}


bool Request::checkPort(std::vector<Server>::iterator it, int port)
{
	std::vector<int> ports = it->getPorts();
	std::vector<int>::iterator it_port;
	for (it_port = ports.begin(); it_port != ports.end(); it_port++)
	{
		if (port == *it_port)
			return (1);
	}
	return (0);
}

int Request::findServer() // the server must be found, otherwise, send error message (normally it never happens)
{
	std::vector<Server>::iterator it;
	// std::cout << "_requestHeader.getRequestHost() is " << _requestHeader.getRequestHost() << std::endl;
	for (it = _servers->begin(); it != _servers->end(); it++)
	{
		if (_requestHeader.getRequestHost() == it->getServerName() && checkPort(it, stoi(_requestHeader.getRequestPort())))
		{
			_requestServer = &(*it);
			// std::cout << "_requestServer is " << _requestServer->getServerName() << std::endl;
			return (0);
		}
	}
	// std::cout << "_requestServer is " << _requestServer << std::endl;
	throw Exception_Request("server(host) is not found", 400, 0);
}

int Request::findDirSetting_checkCGI()
{
	std::string requestLocation;
	std::vector<DirSettings> *list_dirsetting = _requestServer->getOptDirSettings();
	std::vector<DirSettings> *list_CGIdirsetting = _requestServer->getCGIDirSettings();

	requestLocation = _requestHeader.getRequestLocation();
	if (requestLocation.back() == '/')
		requestLocation.pop_back();

	for (; requestLocation != ""; deleteStringEnd(&requestLocation, "/"))
	{
		std::string path_dir = _requestServer->getRootDir();
		path_dir.pop_back();
		path_dir += requestLocation;
		for (std::vector<DirSettings>::iterator it = list_dirsetting->begin(); it != list_dirsetting->end(); it++)
		{

			if (path_dir == it->getLocation())
			{
				// std::cout << "!! requestDir found in _optDirSettings -> " << path_dir << std::endl;
				_requestDirSetting = &(*it);
				return (0);
			}
		}
		for (std::vector<DirSettings>::iterator it = list_CGIdirsetting->begin(); it != list_CGIdirsetting->end(); it++)
		{
			if (path_dir == it->getLocation())
			{
				// std::cout << "!! requestDir found, in _cgiDirSettings -> " << path_dir << std::endl;
				_requestDirSetting = &(*it);
				_requestCGI = 1;
				return (0);
			}
		}
	}
	// std::cout << "!!! requestDir is _rootDirSettings  " << _requestServer->getRootDirSettings()->getLocation() << std::endl;
	_requestDirSetting = _requestServer->getRootDirSettings();
	return (0);
}

int Request::checkRedirect()
{
	if (_requestDirSetting->getRedirect().size() != 0)
	{
		// std::cout << "!!!! redirect is set " << _requestDirSetting->getRedirect().begin()->first << " " << _requestDirSetting->getRedirect().begin()->second << std::endl;
		// _requestSocket->setRedirect(_requestDirSetting->getRedirect().begin()->second);
		throw Exception_Request("Redirect is set", 0, _requestDirSetting->getRedirect().begin()->first);
	}
	return (0);
}

int Request::checkProtocol()
{
	if (_requestHeader.getHTTPProtocol() != "HTTP/1.1")
	{
		throw Exception_Request("Protocol not allowed", 400, 0);
	}
	return (0);
}

int Request::setRequestBodyLength() // body length may not exist, depends on the request.
{
	std::map<std::string, std::string>::iterator it;

	it = _requestHeader.getHeaderOthers()->find("Content-Length");
	if (it != _requestHeader.getHeaderOthers()->end())
		_requestBodyLength = stoi(it->second);
	// std::cout << "!!!_requestBodyLength is " << _requestBodyLength << std::endl;
	if (_requestBodyLength > _requestDirSetting->getMaxBodySize())
		throw Exception_Request("Content-Length is too much", 413, 0);
	return (0);
}

int Request::setRequestBody() // request body may not exist, depends on the request.
{
	std::string str_read = vectorToString(&_dataR);
	std::string slicepart;

	try
	{
		slicepart = splitString(&str_read, "\r\n\r\n");
		_requestBody = str_read;
	}
	catch (const std::exception &e)
	{
		// this case, there is no request body (only header)
	}
	if (_requestBody.size() > _requestDirSetting->getMaxBodySize())
		throw Exception_Request("Content-Length is too much", 413, 0);
	return (0);
}


int Request::checkMethod()
{
	std::vector<std::string> allowedmethod = _requestDirSetting->getMethods();
	// std::cout << "!!!! Dir is " <<  _requestDirSetting->getLocation() << std::endl;
	for (std::vector<std::string>::iterator it = allowedmethod.begin(); it != allowedmethod.end(); it++)
	{
		if (_requestHeader.getRequestMethod() == *it)
			return (0);
	}
	throw Exception_Request("Method not allowed", 405, 0);
}


bool Request::checkCGI()
{
	if (_requestCGI)
		return (1);
	return (0);
}

// exception
Request::Exception_Request::Exception_Request() : _error_msg("Request setting failed"), _error_num(0), _status_num(0) {}
Request::Exception_Request::Exception_Request(const char *error_msg, int err, int status) : _error_msg(error_msg), _error_num(err), _status_num(status)
{
}

const char *Request::Exception_Request::what() const _NOEXCEPT
{
	std::cout << "Error : in Request : ";
	return (_error_msg);
}
