#ifndef REQUEST_HPP
# define REQUEST_HPP
# include <vector>
# include <map>
# include <iostream>
# include <sys/socket.h>
# include <sys/event.h>
# include <sys/stat.h>
# include <stdlib.h>
# include "macro.hpp"
# include "RequestHeader.hpp"

class Server;
class RequestHeader;
class Request
{
	private:
		std::vector<char>	_dataR;
		int					_sizeR;
		RequestHeader		_requestHeader;
		std::string			_requestContentType;
		size_t				_requestBodyLength;
		std::string			_requestBody;
		bool				_requestShowList;
		bool				_requestCGI;

		SocketConnect			*_requestSocket;
		std::vector<Server>		*_servers;
		Server					*_requestServer;
		DirSettings				*_requestDirSetting;

	public:
		Request();
		~Request();
		Request &operator=(const Request &source);
		Request(const Request &source);

		int				getSizeR() const;
		RequestHeader	*getRequestHeader();
		std::string		getRequestContentType() const;
		int				getRequestBodyLength() const;
		std::string		getRequestBody() const;
		DirSettings		*getRequestDirSettings();
		Server			*getRequestServer();
		
		void			printDataR();
		void			printSizeR();
		void			printRequestParsedData();
		bool			getRequestShowList();

		void			addDataR(char c);
		void 			setRequest(std::vector<Server> *list_server, SocketConnect *socket);
		int 			setRequestHeader();
		int 			setRequestContentType();
		bool			checkPort(std::vector<Server>::iterator it, int port);
		int 			findServer();
		int 			findDirSetting_checkCGI();
		int 			checkRedirect();
		int 			checkProtocol();
		int 			setRequestBodyLength();
		int 			setRequestBody();
		int 			checkMethod();
		bool			checkCGI();


	// exception
	class Exception_Request : public std::exception
	{
		private:
		    const char *_error_msg;

		public:
		    int _error_num;
			int _status_num;
		    Exception_Request();
		    Exception_Request(const char *exception_msg, int err, int status);
		    const char *what() const _NOEXCEPT;
	};
};

#endif
