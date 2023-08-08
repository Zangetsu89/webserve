#ifndef REQUEST_HPP
# define REQUEST_HPP
# include <vector>
# include <map>
# include <iostream>
# include <sys/socket.h>
# include <sys/types.h>
# include <sys/event.h>
# include <sys/time.h>
# include <stdlib.h>
# include "macro.hpp"
# include "Server.hpp"
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
	int				_requestBodyLength;
	std::string		_requestBody;
	bool			_requestShowList;

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
	RequestHeader	*getRequestHeader();
	void			printDataR();
	void			printSizeR();
	bool			getRequestShowList();

	int 			setRequest(std::vector<Server> *list_server, SocketConnect *socket);
	int 			readRequest();
	int 			setRequestHeader();
	int 			setRequestBodyLength();
	int 			setRequestBody();
	int 			findServer();
	int 			findDirSetting();
	int 			checkMethod();
	int 			checkProtocol();
	int 			findResponseFile();

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
