#include "../include/CgiHandler.hpp"
#include "../include/SocketConnect.hpp"
#include "../include/Request.hpp"
#include "../include/DirSettings.hpp"
#include "../include/Response.hpp"
#include "../include/util.hpp"

// The respons generate function should see if the path is a directory or
// file. If directory, check if there is an index file, if yes, return the index
// file, if not, see the directory permission. If the permission is true,
// read the directory and give back the list of content.
// If not a directory, check if plain text or a file: actually python handles this allready
// all plain text POST get saved in html/user directory?
// if GET searches for a plain file, look for the file in html/user directory?
// the POST method should contain plain text message?

CgiHandler::CgiHandler(SocketConnect *socket)
{
	this->_socket = socket;
	this->_request = _socket->getClientRequest();
	this->_response = _socket->getClientResponse();
	this->_cgiDir = _request->getRequestDirSettings()->getCgiDir();
	makeCgiEnv();
	makeCgiArgv();
}

CgiHandler::CgiHandler(CgiHandler const &source)
{
	*this = source;
}

CgiHandler::~CgiHandler()
{
}

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
	}
	return (*this);
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
	std::string cgiExtension = _request->getRequestDirSettings()->getCgiExtension();

	if (cgiExtension == "py")
		_cgiArgv.push_back("python3");
	else if (cgiExtension == "php")
		_cgiArgv.push_back("php");

	std::string filepath = _response->getResponseFilePath();
	_cgiArgv.push_back(filepath);
}

int CgiHandler::prepareResponse()
{
	std::string method = _request->getRequestHeader()->getRequestMethod();

	int err;
	int fd_exe[2];	// for executing
	int fd_post[2]; // only used for POST method (pass the Response body to the CGI through STDIN)

	try
	{
		if (pipe(fd_exe) == -1 || pipe(fd_post) == -1)
			throw ERR_CgiHandler("pipe failed", 500);

		int pid = fork();
		if (pid < 0)
			throw ERR_CgiHandler("fork failed", 500);
		if (pid == 0) // child process to excute CGI
		{
			if (method == "POST")
			{
				dup2(fd_post[0], 0);
				close(fd_post[1]);
				dup2(fd_exe[1], 1);
				close(fd_exe[0]);
			}
			else
			{
				dup2(fd_exe[1], 1);
				close(fd_exe[0]);
				close(fd_post[0]);
				close(fd_post[1]);
			}
			static char **cgiArray = stringCharArray(_cgiArgv);
			static char **cgiEnvArray = stringCharArray(_cgiEnv);
			execve(_cgiDir.c_str(), cgiArray, cgiEnvArray);
			std::cout << "execve failed" << std::endl;
			cleanStringCharArray(cgiArray);
			cleanStringCharArray(cgiEnvArray);
			exit(1);
		}
		else
		{
			int length = _request->getRequestBodyLength();
			std::string postdata = _request->getRequestBody();

			if (method == "POST")
			{
				close(fd_post[0]);
				close(fd_exe[1]);

				char *posting = reinterpret_cast<char *>(postdata.data());
				if (write(fd_post[1], posting, length) < 0)
					throw ERR_CgiHandler("cgi data generate failed!", 500);
				close(fd_post[1]);
			}
			else
			{
				close(fd_post[0]);
				close(fd_post[1]);
				close(fd_exe[1]);
			}

			int r = 1;
			char c;
			while (r == 1)
			{
				r = read(fd_exe[0], &c, 1);
				if (r < 0)
					throw ERR_CgiHandler("cgi data generate failed", 500);
				_response->addCtoResponseBody(c);
				_response->addResponseContentLength(r);
			}
			waitpid(pid, &err, 0);
			if (err != 0)
				throw ERR_CgiHandler("wait failed", 500);
			close(fd_exe[0]);
		}
		_socket->setStatus(200);
		return (0);
	}
	catch (ERR_CgiHandler &e)
	{
		std::cerr << e.what() << '\n';
		_socket->setError(e._error_num);
		return (e._error_num);
	}
}

CgiHandler::ERR_CgiHandler::ERR_CgiHandler() : _error_msg("Request setting failed"), _error_num(0) {}
CgiHandler::ERR_CgiHandler::ERR_CgiHandler(const char *error_msg, int err) : _error_msg(error_msg), _error_num(err)
{
}

const char *CgiHandler::ERR_CgiHandler::what() const _NOEXCEPT
{
	std::cout << "Error : in CgiHandler : ";
	return (_error_msg);
}
