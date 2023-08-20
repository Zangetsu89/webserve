//
// Request
// Request class stores client's request information
//

#include "../../include/RequestHeader.hpp"
#include "../../include/Request.hpp"
#include "../../include/SocketConnect.hpp"
#include "../../include/util.hpp"


Request::Request():_requestBodyLength(0),_requestShowList(0) {
}

Request::~Request() {
}

Request& Request::operator=(const Request &source)
{
	if (this != &source)
	{
		_requestHeader = source._requestHeader;
		_requestFilePath = source._requestFilePath;
		_requestBodyLength = source._requestBodyLength;
		_requestBody = source._requestBody;
		_requestShowList = source._requestShowList;
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

bool	Request::getRequestShowList()
{
	return (_requestShowList);
}

int Request::setRequest(std::vector<Server> *list_server, SocketConnect *socket)
{
	_servers = list_server;
	_requestSocket = socket;
	
	try
	{
        try {
		    readRequest();
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
        try {
		    setRequestHeader();
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
        try {
		    setRequestBodyLength();
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
        try {
		    setRequestBody();
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
        try {
		    findServer();
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
        try {
		    findDirSetting();
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
        try {
		    checkRedirect();
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
        try {
		    checkProtocol();
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
        try {
		    findResponseFile();
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
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
		throw ERR_Request("The first line of request is wrong", res);
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

bool	Request::checkPort(std::vector<Server>::iterator it, int port)
{
	std::vector<int>	ports = it->getPorts();
	std::vector<int>::iterator it_port;
	for (it_port = ports.begin(); it_port != ports.end(); it_port++)
	{
		if (port == *it_port)
			return (1);
	}
	return (0);
}

int	Request::findServer()
{
	std::vector<Server>::iterator	it;
	std::cout << "_requestHeader.getRequestHost() is " << _requestHeader.getRequestHost() << std::endl;
	for (it = _servers->begin(); it != _servers->end(); it++)
	{
        std::cout << "Port is " << _requestHeader.getRequestPort() << std::endl;
		if (_requestHeader.getRequestHost() == it->getServerName() && checkPort(it, stoi(_requestHeader.getRequestPort())))
		{
            std::cout << &(*it) << std::endl;
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
	std::cout << "!!! requestDir is gotten " << _requestServer->getRootDirSettings()->getLocation() << std::endl;
	return (0);
}

int	Request::checkRedirect()
{
	if (_requestServer->getRootDirSettings()->getRedirect().size() != 0)
	{
		std::cout << "!!!! redirect is set " << _requestServer->getRootDirSettings()->getRedirect().begin()->first << " " << _requestServer->getRootDirSettings()->getRedirect().begin()->second << std::endl;
		_requestSocket->setRedirect(_requestServer->getRootDirSettings()->getRedirect().begin()->second);
		throw ERR_Request("Redirect is set", _requestServer->getRootDirSettings()->getRedirect().begin()->first);
	}
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
	struct stat	status;
	std::string filepath = _requestHeader.getRequestLocation();
	std::cout << "filepath is " << filepath << std::endl;
	if (filepath.back() == '/')
	{
		filepath.pop_back();
        std::cout << "HEREEEE" << std::endl;
        std::cout << "TESTING" << _requestServer->getRootDir() << std::endl;
        std::cout << "TESTING 2" << _requestDirSetting->getIndexPage() << std::endl;
		_requestFilePath = _requestServer->getRootDir() + _requestDirSetting->getIndexPage();
		std::cout << "!!! _requestFilePath is " << _requestFilePath << std::endl;
		if (stat(_requestFilePath.c_str(), &status) != 0)
		{
			if (!_requestDirSetting->getDirPermission())
				throw ERR_Request("file not found and showing list not allowed", 403);
			_requestShowList = 1;
		}
		std::cout << "!!! _requestShowList is " << _requestShowList << std::endl;
		return (0);
	}
	if (filepath.front() == '/')
		filepath.erase(0, 1);
	_requestFilePath = _requestServer->getRootDir() + filepath;
	std::cout << "!! _requestFilePath is " << _requestFilePath << std::endl;
	if (stat(_requestFilePath.c_str(), &status) != 0)
		throw ERR_Request("file not found", 404);
	if ((status.st_mode & S_IFMT) == S_IFREG)
	{
		std::cout << "This is a file: _requestFilePath(index) is " << _requestFilePath << std::endl;
		return (0);
	}
		
	if ((status.st_mode & S_IFMT) == S_IFDIR)
	{
		_requestFilePath = _requestFilePath + "/" + _requestDirSetting->getIndexPage();
		std::cout << "This is a directory: _requestFilePath is " << _requestFilePath << std::endl;
		if (stat(_requestFilePath.c_str(), &status) != 0)
		{
			if (!_requestDirSetting->getDirPermission())
				throw ERR_Request("file not found and showing list not allowed", 403);
			_requestShowList = 1;
		}
		return (0);
	}
	std::cout << "!! _requestShowList is " << _requestShowList << std::endl;
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

