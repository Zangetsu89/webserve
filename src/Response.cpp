#include <vector>
#include <string>
#include "../include/macro.hpp"
#include "../include/util.hpp"
#include "../include/status.hpp"
#include "../include/Response.hpp"
#include "../include/CgiHandler.hpp"
#include "../include/SocketConnect.hpp"

Response::Response(): _responseStatusCode(0), _responseContentLength(0), _responseSize(0), _responseSent(0) 
{}

Response::Response(Request *request)
{
	this->_request = request;
}

Response::Response(Response const &src)
{
	*this = src;
}

Response::~Response() {}

Response &Response::operator=(Response const &rhs)
{
	if (this != &rhs)
	{
		this->_request = rhs._request;
		this->_responseSocket = rhs._responseSocket;
		this->_responseFilePath = rhs._responseFilePath;
		this->_responseStatusCode = rhs._responseStatusCode;
		this->_responseStatusMessage = rhs._responseStatusMessage;
		this->_responseContentType = rhs._responseContentType;
		this->_responseContentLength = rhs._responseContentLength;
		this->_responseBody = rhs._responseBody;
		this->_responseHeader = rhs._responseHeader;
		this->_responseFullData = rhs._responseFullData;
		this->_responseSize = rhs._responseSize;
		this->_responseSent = rhs._responseSent;
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

void Response::addCtoResponseBody(char c)
{
	_responseBody.push_back(c);
}

void Response::addResponseContentLength(int i)
{
	_responseContentLength += i;
}


void Response::makeResponse(Request *request, SocketConnect *socket)
{
	this->_request = request;
	this->_responseSocket = socket;
	makeResponseBody();
	makeResponseHeader();
	makeResponseFullData();
}

void Response::makeResponseBody()
{
	int err = _responseSocket->getErrorNum();

	if (err)
	{
		generateErrorBody();
		return;
	}

	switch (checkResponseStatus())
	{
	case ISFILE:
		if (_request->checkCGI())
		{
			CgiHandler cgidata(_responseSocket);
			if (cgidata.prepareResponse())
				generateErrorBody();
		}
		else
			readResponseFile();
		break;
	case NOTFOUND:
		generateErrorBody();
		break;
	case SHOWLIST:
		generateDirectoryListing();
		break;
	}
}

void Response::makeResponseHeader()
{
	int err = _responseSocket->getErrorNum();
	int status = _responseSocket->getStatusNum();
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
		if (!_request->checkCGI())
			_responseContentType = get_MIMETypes(_responseFilePath);
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
	if (requestlocation.front() == '/')
		requestlocation.erase(0, 1);

	std::string requestPath = _request->getRequestServer()->getRootDir() + requestlocation;
	if (requestPath.back() == '/') // request is directory
	{
		// std::cout << "requestPath is a directory : " << requestPath << std::endl;
		std::string requestDir = requestPath;
		requestPath = requestPath + _request->getRequestDirSettings()->getIndexPage();
		// std::cout << "new requestPath (with index) is : " << requestPath << std::endl;
		struct stat status;
		if (stat(requestPath.c_str(), &status) == 0)
		{
			_responseFilePath = requestPath;
			return (ISFILE);
		}
		if (_request->getRequestDirSettings()->getDirPermission())
		{
			_responseFilePath = requestDir;
			return (SHOWLIST);
		}
		std::cout << "requestPath is not found and showing list is not allowed : " << requestPath << std::endl;
		_responseSocket->setError(403);
		return (NOTFOUND);
	}
	else
	{
		struct stat status;
		if (stat(requestPath.c_str(), &status) == 0)
		{
			// std::cout << "requestPath file is found : " << requestPath << std::endl;
			if ((status.st_mode & S_IFMT) == S_IFDIR)
			{
				// std::cout << "requestPath is a directory : " << requestPath << std::endl;
				std::string requestDir = requestPath;
				requestPath = requestPath + _request->getRequestDirSettings()->getIndexPage();
				// std::cout << "new requestPath2 (with index) is : " << requestPath << std::endl;
				struct stat status;
				if (stat(requestPath.c_str(), &status) == 0)
				{
					_responseFilePath = requestPath;
					return (ISFILE);
				}
				if (_request->getRequestDirSettings()->getDirPermission())
				{
					_responseFilePath = requestDir;
					return (SHOWLIST);
				}

				// std::cout << "requestPath file2 is not found : " << requestPath << std::endl;
				_responseSocket->setError(404);
				return (NOTFOUND);
			}
			_responseFilePath = requestPath;
			return (ISFILE);
		}
		else
		{
			// std::cout << "requestPath file2 is not found : " << requestPath << std::endl;
			_responseSocket->setError(404);
			return (NOTFOUND);
		}
	}
	return (NOTFOUND);
}

void Response::readResponseFile()
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
		_responseSocket->setStatus(200);
	}
	else
	{
		std::cout << "can't read a file" << std::endl;
		_responseSocket->setError(400);
	}
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
		_responseSocket->setError(403);
		return;
	}
	addString_toVectorChar(&_responseBody, "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><title>Hello World</title></head><body><ul>");
	for (auto file : files)
	{
		file = "<li><a href=\"./" + file + "\"> " + file + "</a></li>";
		addString_toVectorChar(&_responseBody, file);
	}
	addString_toVectorChar(&_responseBody, "</ul></body></html>");
	// std::cout << "List is made! " << _responseFilePath.c_str() << std::endl;
	// for (int i = 0; i < (int)_responseBody.size(); i++)
	// 	std::cout << _responseBody[i];
	// std::cout << std::endl;
	_responseContentLength = _responseBody.size();
	_responseSocket->setStatus(200);
}

void Response::generateErrorBody()
{
	int err = _responseSocket->getErrorNum();
	std::string filetype = "Content-Type: text/html; charset=UTF-8\r\n\r\n";
	std::string rootPath = _request->getRequestServer()->getRootDir();
	rootPath.pop_back();

	std::map<int, std::string> err_map = _request->getRequestDirSettings()->getErrorPage();
	std::map<int, std::string>::iterator it;
	it = err_map.find(err);
	if (it != err_map.end())
	{
		// std::cout << "Error file is " << it->second << std::endl;
		_responseFilePath = rootPath + it->second;
		// std::cout << "Error file path is " << _responseFilePath << std::endl;
		readResponseFile();
		return;
	}
	std::string message = set_ErrorStatus(err);
	std::string errbody = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><title>";
	errbody = errbody + std::to_string(err) + " " + message + "</title></head><body>" + std::to_string(err) + " " + message + "<br><br>HAHAHA, this is default error page!</body></html>";
	// std::cout << "Error body is made " << std::endl;
	for (size_t i = 0; i < errbody.size(); i++)
	{
		_responseBody.push_back(errbody[i]);
		std::cout << _responseBody[i];
	}
	_responseContentLength = _responseBody.size();
}

int Response::sendResponse(int socket)
{
	int sent;
	int sendBUFF = BUFFSIZE;
	if (_responseSize < BUFFSIZE)
		sendBUFF = _responseSize;
	sent = send(socket, &_responseFullData[_responseSent], sendBUFF, 0);
	if (sent < 0)
		throw std::invalid_argument("failed to send file");
	_responseSize -= sent;
	_responseSent += sent;
	return (_responseSize);
}
