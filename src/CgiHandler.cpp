#include "../include/CgiHandler.hpp"
#include "../include/SocketConnect.hpp"
#include "../include/Request.hpp"
#include "../include/DirSettings.hpp"
#include "../include/Response.hpp"
#include "../include/util.hpp"
#include <signal.h>

CgiHandler::CgiHandler(){}

CgiHandler::CgiHandler(CgiHandler const &source)
{
	*this = source;
}

CgiHandler::~CgiHandler(){}

CgiHandler &CgiHandler::operator=(CgiHandler const &source)
{
	if (this != &source)
	{
		this->_socket = source._socket;
		this->_request = source._request;
		this->_response = source._response;
		this->_cgiDir = source._cgiDir;
		this->_cgiProgram = source._cgiProgram;
		this->_cgiEnv = source._cgiEnv;
		this->_cgiArgv = source._cgiArgv;
		this->_cgiPostKevent = source._cgiPostKevent;
		this->_cgiExecKevent = source._cgiExecKevent;
	}
	return (*this);
}

void	CgiHandler::setCgiHandler(SocketConnect *socket)
{
	_socket = socket;
	_request = _socket->getSocketRequest();
	_response = _socket->getSocketResponse();
	_cgiDir = _request->getRequestCGI().second;
	_cgiProgram = _request->getRequestCGI().first;
	if (_cgiProgram == ".py")
		_cgiProgram = "python3";
	else
		_cgiProgram.erase(0,1); // delete . from extention
	makeCgiEnv();
	makeCgiArgv();
}

static char **stringCharArray(std::vector<std::string> strVector)
{
	char **Array;
	Array = (char **)malloc(sizeof(char *) * strVector.size() + 1);
	for (unsigned int i = 0; i < strVector.size(); i++)
	{
		char *InsideArray = (char *)malloc(strVector[i].size() + 1);
		strcpy(InsideArray, strVector[i].c_str());
		InsideArray[strVector[i].size()] = '\0';
		Array[i] = InsideArray;
	}
	Array[strVector.size()] = NULL;
	return ((char **)Array);
}

void cleanStringCharArray(char **Array)
{
	for (size_t i = 0; Array[i]; i++)
		delete (Array[i]);
	delete (Array);
}

void CgiHandler::makeCgiEnv()
{
	RequestHeader *Header = _request->getRequestHeader();

	_cgiEnv.push_back("REQUEST_METHOD=" + Header->getRequestMethod());
	_cgiEnv.push_back("SERVER_NAME=" + _request->getRequestServer()->getServerName());
	_cgiEnv.push_back("SERVER_PROTOCOL=HTTP/1.1");
	if (_request->getRequestBodyLength())
		_cgiEnv.push_back("CONTENT_LENGTH=" + std::to_string(_request->getRequestBodyLength()));
	if (_request->getRequestContentType().size())
		_cgiEnv.push_back("CONTENT_TYPE=" + _request->getRequestContentType());
	if (Header->getRequestLocationParametor().size())
		_cgiEnv.push_back("QUERY_STRING=" + Header->getRequestLocationParametor());
	_cgiEnv.push_back("SCRIPT_FILENAME=" + _response->getResponseFilePath());
	_cgiEnv.push_back("SCRIPT_NAME=" + Header->getRequestLocation());
}

void CgiHandler::makeCgiArgv()
{
	_cgiArgv.push_back(_cgiProgram);
	std::string filepath = _response->getResponseFilePath();
	_cgiArgv.push_back(filepath);
}


void	CgiHandler::setCGI_FD()
{
	std::string method = _request->getRequestHeader()->getRequestMethod();

	try
	{
		if (method == "POST")
		{
			std::cout << std::endl << "[FD adding to CGI(POST)] "<< std::endl;
			int fd_exe[2];	// for executing
			int fd_post[2]; // to pass the Response body to the CGI through STDIN

			if (pipe(fd_exe) == -1 || pipe(fd_post) == -1)
				throw ERR_CgiHandler("pipe failed", 500);
			int pid = fork();
			if (pid < 0)
				throw ERR_CgiHandler("fork failed", 500);
			if (pid == 0) // child process to excute CGI
			{
				dup2(fd_post[0], 0);
				close(fd_post[1]);
				dup2(fd_exe[1], 1);
				close(fd_exe[0]);
				static char **cgiArray = stringCharArray(_cgiArgv);
				static char **cgiEnvArray = stringCharArray(_cgiEnv);
				execve(_cgiDir.c_str(), cgiArray, cgiEnvArray);
				cleanStringCharArray(cgiArray);
				cleanStringCharArray(cgiEnvArray);
				exit(1);
			}
			else
			{
				close(fd_post[0]);
				close(fd_exe[1]);
				_socket->getSocketResponse()->setResponseCGIFD(fd_exe[0]);
				EV_SET(&_cgiPostKevent, fd_post[1], EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, _response);
				kevent(_socket->getKqueueNum(), &_cgiPostKevent, 1, NULL, 0, NULL);
			}
		}
		else
		{
			std::cout << std::endl << "[FD adding to CGI(NOT POST)] "<< std::endl;
			int fd_exe[2];	// for executing

			if (pipe(fd_exe) == -1)
				throw ERR_CgiHandler("pipe failed", 500);
			int pid = fork();
			if (pid < 0)
				throw ERR_CgiHandler("fork failed", 500);
			if (pid == 0) // child process to excute CGI
			{
				dup2(fd_exe[1], 1);
				close(fd_exe[0]);
				static char **cgiArray = stringCharArray(_cgiArgv);
				static char **cgiEnvArray = stringCharArray(_cgiEnv);
				execve(_cgiDir.c_str(), cgiArray, cgiEnvArray);
				cleanStringCharArray(cgiArray);
				cleanStringCharArray(cgiEnvArray);
				exit(1);
			}
			else
			{
				close(fd_exe[1]);
				EV_SET(&_cgiExecKevent, fd_exe[0], EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, _response);
				kevent(_socket->getKqueueNum(), &_cgiExecKevent, 1, NULL, 0, NULL);
			}
		}
	}
	catch (ERR_CgiHandler &e)
	{
		_socket->getSocketResponse()->setError(e._error_num);
	}
}

int CgiHandler::writePost(int socket)
{
	int sent;
	int sendBUFF = BUFFSIZE;
	
	int	datarest = _request->getRequestBodyLength() - _request->getRequestBodySentCGI();
	std::string *postdata = _request->getRequestBody();

	if (datarest < BUFFSIZE)
		sendBUFF = datarest;
	char	buffer[sendBUFF];
	postdata->copy(buffer, sendBUFF, _request->getRequestBodySentCGI());

	sent = write(socket, &buffer , sendBUFF);
	if (sent < 0)
		throw std::invalid_argument("failed to send file");
	_request->addRequestBodySentCGI(sent);
	datarest = _request->getRequestBodyLength() - _request->getRequestBodySentCGI();
	return (_request->getRequestBodyLength() - _request->getRequestBodySentCGI());
}

struct kevent	*CgiHandler::getCgiPostKevent()
{
	return (&_cgiPostKevent);
}

struct kevent	*CgiHandler::getCgiExecKevent()
{
	return (&_cgiExecKevent);
}

CgiHandler::ERR_CgiHandler::ERR_CgiHandler() : _error_msg("Request setting failed"), _error_num(0) {}
CgiHandler::ERR_CgiHandler::ERR_CgiHandler(const char *error_msg, int err) : _error_msg(error_msg), _error_num(err)
{
}

const char *CgiHandler::ERR_CgiHandler::what() const noexcept
{
	std::cout << "Error : in CgiHandler : ";
	return (_error_msg);
}
