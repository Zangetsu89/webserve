#include <vector>
#include <string>
#include <sys/stat.h>
#include <fcntl.h>
#include "../include/macro.hpp"
#include "../include/util.hpp"
#include "../include/status.hpp"
#include "../include/Response.hpp"
#include "../include/CgiHandler.hpp"
#include "../include/SocketConnect.hpp"

Response::Response(): _responseContentLength(0), _responseSize(0), _responseSent(0), _responseFD(0), _responseCGIFD(0), _responseErrorNum(0), _responseStatusNum(0)
{}

Response::Response(Request *request)
{
	this->_request = request;
}

Response::Response(Response const &src)
{
	*this = src;
}

Response::~Response(){}

Response &Response::operator=(Response const &rhs)
{
	if (this != &rhs)
	{
		this->_request = rhs._request;
		this->_responseSocket = rhs._responseSocket;
		this->_responseFilePath = rhs._responseFilePath;
		this->_responseContentType = rhs._responseContentType;
		this->_responseContentLength = rhs._responseContentLength;
		this->_responseBody = rhs._responseBody;
		this->_responseHeader = rhs._responseHeader;
		this->_responseFullData = rhs._responseFullData;
		this->_responseSize = rhs._responseSize;
		this->_responseSent = rhs._responseSent;
		this->_responseFD = rhs._responseFD;
		this->_responseCGIFD = rhs._responseCGIFD;
		this->_responseErrorNum = rhs._responseErrorNum;
		this->_responseStatusNum = rhs._responseStatusNum;
		this->_responseFileKevent = rhs._responseFileKevent;
	}
	return *this;
}

int Response::getResponseSize() const
{
	return (_responseSize);
}

std::string Response::getResponseFilePath() const
{
	return (_responseFilePath);
}

int Response::getResponseContentLength() const
{
	return (_responseContentLength);
}

int	Response::getCurrentResponseBodySize() const
{
	return (_responseBody.size());
}

int	Response::getResponseFD() const
{
	return (_responseFD);
}

int	Response::getResponseCGIFD() const
{
	return (_responseCGIFD);
}

int	Response::getErrorNum() const
{
	return (_responseErrorNum);
}

int	Response::getStatusNum() const
{
	return (_responseStatusNum);
}

SocketConnect   *Response::getResponseSocket()
{
	return (_responseSocket);
}


void Response::setError(int err)
{
	if (!_responseErrorNum)
		_responseErrorNum = err;
}

void Response::setStatus(int status)
{
	_responseStatusNum = status;
}

void Response::setResponseCGIFD(int fd)
{
	_responseCGIFD = fd;
}


void Response::addCtoResponseBody(char c)
{
	_responseBody.push_back(c);
}

void Response::addResponseContentLength(int i)
{
	_responseContentLength += i;
}

bool Response::setResponseBody(SocketConnect *socket)
{
	this->_responseSocket = socket;
	this->_request = socket->getSocketRequest();

	int err = _responseErrorNum;

	if (err)
	{
		generateErrorBody();
		return (1);
	}

	if (checkRedirectSet())
	{
		generateRedirectData();
		return (1);
	}

	switch (checkResponseStatus())
	{
		case ISFILE:
			if (_request->checkCGI())
			{
				std::cout << std::endl << "[Setting for CGI] "<< std::endl;
				_responseSocket->getsocketCgiHandler()->setCgiHandler(_responseSocket);
				std::cout << std::endl << "[Setting for CGIFD] "<< std::endl;
				_responseSocket->getsocketCgiHandler()->setCGI_FD();
			}
			else
				openFD_ResponseFile();
			break;
		case NOTFOUND:
			generateErrorBody();
			return (1);
		case SHOWLIST:
			generateDirectoryListing();
			return (1);
	}
	return (0);
}

bool	Response::checkRedirectSet()
{
	if (_responseStatusNum >= 300 && _responseStatusNum < 400)
		return (1);
	return (0);
}


void Response::makeResponseHeader()
{
	int err = _responseErrorNum;
	int status = _responseStatusNum;
	std::string message;
	std::string responseHeader_str;
	_responseContentType = "text/html";

	if (err)
	{
		message = set_ErrorStatus(err);
		responseHeader_str = "HTTP/1.1 " + std::to_string(err) + " " + message + "\r\n";
		responseHeader_str = responseHeader_str + "Content-Type: text/html; charset=UTF-8\r\n\r\n";
	}
	else
	{
		if (_request->getRequestCGI().second.empty())
			_responseContentType = get_MIMETypes(_responseFilePath);
		else
			_responseContentType = "text/html";
		message = set_Status(status);
		responseHeader_str = "HTTP/1.1 " + std::to_string(status) + " " + message + "\r\n";
		responseHeader_str = responseHeader_str + "Content-Type: " + _responseContentType + "\r\n\r\n";
	}

	for (size_t i = 0; i < responseHeader_str.size(); i++)
	{
		_responseHeader.push_back(responseHeader_str[i]);
	}
}

void Response::makeResponseFullData()
{
	_responseHeader.insert(_responseHeader.end(), _responseBody.begin(), _responseBody.end());
	_responseFullData = _responseHeader;
	_responseSize = _responseFullData.size();
}

int Response::checkResponseStatus()
{
	std::string requestlocation = this->_request->getRequestHeader()->getRequestLocation();
	std::string requestPath = _request->getRequestServer()->getRootPath() + requestlocation;
	struct stat status;

	if (requestPath.back() != '/')
	{
		// check if it is a file 	
		if (stat(requestPath.c_str(), &status) == 0)
		{
			if ((status.st_mode & S_IFMT) == S_IFDIR)
			{
				std::string requestDir = requestPath;
				std::vector<std::string> indexlist = _request->getRequestDirSettings()->getIndexPage();
				for (size_t i = 0; i < indexlist.size() ; i++)
				{
					_responseFilePath = requestDir + "/" + indexlist[i];
					if (stat(_responseFilePath.c_str(), &status) == 0)			
						return (ISFILE);
				}
				if (_request->getRequestDirSettings()->getDirPermission())
				{
					_responseFilePath = requestDir;
					return (SHOWLIST);
				}
				_responseErrorNum = 403;
				return (NOTFOUND);
			}
			_responseFilePath = requestPath;
			return (ISFILE);
		}
		_responseErrorNum = 404;
		return (NOTFOUND);
	}
	else	// request is directory
	{
		if (stat(requestPath.c_str(), &status) != 0)
		{
			_responseErrorNum = 404;
			return (NOTFOUND);
		}
		std::string requestDir = requestPath;
		std::vector<std::string> indexlist = _request->getRequestDirSettings()->getIndexPage();
		struct stat status;
		for (size_t i = 0; i < indexlist.size() ; i++)
		{
			_responseFilePath = requestDir + "/" + indexlist[i];
			if (stat(_responseFilePath.c_str(), &status) == 0)			
				return (ISFILE);
		}
		if (_request->getRequestDirSettings()->getDirPermission())
		{
			_responseFilePath = requestDir;
			return (SHOWLIST);
		}
		_responseErrorNum = 403;
		return (NOTFOUND);
	}
}

void Response::readResponseSmallFile()
{
	std::ifstream file(_responseFilePath, std::ifstream::binary);
	if (file)
	{
		file.seekg(0, file.end);
		int length = file.tellg();
		file.seekg(0, file.beg);
		_responseContentLength = length;
		char c;

		for (int i = 0; i < length; i++)
		{
			file.read(&c, 1);
			_responseBody.push_back(c);
		}
		file.close();
		_responseStatusNum = 200;
	}
	else
	{
		std::cout << "can't read a file" << std::endl;
		_responseErrorNum = 400;
	}
}

void Response::openFD_ResponseFile()
{
	_responseFD = open(_responseFilePath.c_str(), O_RDONLY);
	if (_responseFD < 0)
		std::cout << "error!" <<  std::endl;
	EV_SET(&_responseFileKevent, _responseFD, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, this);
	kevent(_responseSocket->getKqueueNum(), &_responseFileKevent, 1, NULL, 0, NULL);
}

int Response::readFile(int fd)
{
	char buff[BUFFSIZE];
	int	readTotal = 0;

	readTotal = read(fd, &buff, BUFFSIZE);
	if (readTotal < 0)
		std::cout << "error! in reading file" <<  std::endl;
	for (int i = 0; i < readTotal; i++)
		_responseBody.push_back(buff[i]);
	return (readTotal);
}


void Response::generateDirectoryListing()
{
	DIR *dir;
	struct dirent *diread;
	std::vector<std::string> files;

	if ((dir = opendir(_responseFilePath.c_str())) != nullptr)
	{
		while ((diread = readdir(dir)) != nullptr)
			files.push_back(diread->d_name);
		closedir(dir);
	}
	else
	{
		_responseErrorNum = 403;
		return;
	}
	addString_toVectorChar(&_responseBody, "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><title>Hello World</title></head><body><ul>");
	for (auto file : files)
	{
		file = "<li><a href=\"./" + file + "\"> " + file + "</a></li>";
		addString_toVectorChar(&_responseBody, file);
	}
	addString_toVectorChar(&_responseBody, "</ul></body></html>");
	_responseContentLength = _responseBody.size();
	_responseStatusNum = 200;
	makeResponseHeader();
	makeResponseFullData();
	_responseSocket->setKevent_WRITE();
}


void Response::generateRedirectData()
{

	int redirect_status = _request->getRequestDirSettings()->getRedirect().first;
	std::string redirect_url = _request->getRequestDirSettings()->getRedirect().second;
	std::string redirect_status_str = std::to_string(redirect_status);
	std::string redirect_message = set_Status(redirect_status);
	std::cout << "Redirect is set " << redirect_url << std::endl;

	std::string redirect_string = "HTTP/1.1 " + redirect_status_str + redirect_message + "\r\nLocation: " + redirect_url.c_str();

	for (size_t i = 0; i < redirect_string.size(); i++)
	{
		_responseFullData.push_back(redirect_string[i]);
		_responseSize++;
	}
	_responseSocket->setKevent_WRITE();
}

void Response::generateErrorBody()
{
	std::cout << std::endl << "[Error generating] err is " << _responseErrorNum << std::endl;
	int err = _responseErrorNum;
	std::string rootPath = _request->getRequestServer()->getRootPath();
	std::map<int, std::string> err_map = _request->getRequestDirSettings()->getErrorPage();
	std::map<int, std::string>::iterator it;

	it = err_map.find(err);
	if (it != err_map.end())
	{
		_responseFilePath = rootPath + it->second;
		std::cout << std::endl << "[Error file found] " << _responseFilePath << std::endl;
		readResponseSmallFile();
	}
	else
	{
		std::string message = set_ErrorStatus(err);
		std::string errbody = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><title>";
		errbody = errbody + std::to_string(err) + " " + message + "</title></head><body>" + std::to_string(err) + " " + message + "<br><br>HAHAHA, this is default error page!</body></html>";

		for (size_t i = 0; i < errbody.size(); i++)
			_responseBody.push_back(errbody[i]);

		_responseContentLength = _responseBody.size();
	}
	makeResponseHeader();
	makeResponseFullData();
	_responseSocket->setKevent_WRITE();
}

int Response::sendResponse(int socket)
{
	int sent;
	int sendBUFF = (_responseSize < BUFFSIZE ? _responseSize : BUFFSIZE ) ;
	sent = send(socket, &_responseFullData[_responseSent], sendBUFF, 0);
	if (sent < 0)
		throw std::invalid_argument("failed to send file");
	_responseSize -= sent;
	_responseSent += sent;
	return (_responseSize);
}
