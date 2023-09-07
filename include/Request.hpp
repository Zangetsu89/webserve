#ifndef REQUEST_HPP
# define REQUEST_HPP
# include <vector>
# include <map>
# include <iostream>
# include <sys/socket.h>
# include <sys/types.h>
# include <sys/event.h>
# include <sys/time.h>
# include <sys/stat.h>
# include <stdlib.h>
# include "macro.hpp"
# include "Server.hpp"
# include "DirSettings.hpp"
# include "RequestHeader.hpp"

class Server;
class RequestHeader;
class Request
{
	friend 			SocketConnect;

	// private member
	private:
	std::vector<char>	_dataR;
	int					_sizeR;

	RequestHeader	_requestHeader;
	std::string		_requestFilePath;
	std::string		_requestContentType;
	int				_requestBodyLength;
	std::string		_requestBody;
	bool			_requestShowList;
    int             _errorNum;

	SocketConnect			*_requestSocket;
	std::vector<Server>		*_servers;
	Server					*_requestServer;
	DirSettings				*_requestDirSetting;

	public:
	Request();
	~Request();
	Request &operator=(const Request &source);
	Request(const Request &source);

	std::string		getRequestFilePath();
	std::string		getRequestContentType();
	int				getRequestBodyLength();
	std::string		getRequestBody();
	RequestHeader	*getRequestHeader();
	void			printDataR();
	void			printSizeR();
	bool			getRequestShowList();
	int				getSizeR();
    int				getRequestErrorNum();
    void			setErrorNum(int num);
	DirSettings		*getRequestDirSettings();

	void			addDataR(char c);
	int 			setRequest(std::vector<Server> *list_server, SocketConnect *socket);
	int 			setRequestHeader();
	int 			setRequestContentType();
	int 			setRequestBodyLength();
	int 			setRequestBody();
	//bool			checkPort(std::vector<Server>::iterator it, int port);
	int 			findServer();
	int 			findDirSetting();
	int 			checkRedirect();
	int 			checkMethod();
	int 			checkProtocol();
	int 			findResponseFile();
	int 			readRequest();

	// exception
	class ERR_Request : public std::exception
	{
		private:
		    const char *_error_msg;

		public:
		    int _error_num;
		    ERR_Request();
		    ERR_Request(const char *error_msg, int err);
		    const char *what() const _NOEXCEPT; // _NOEXCEPT is needed since C++11
	};
};


#endif
