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
		std::vector<char>					_dataR;
		int									_sizeR;
		RequestHeader						_requestHeader;
		std::string							_requestContentType;
		size_t								_requestBodyLength;
		std::string							_requestBody;
		std::pair<std::string, std::string>	_requestCGI;
		size_t								_requestBodySentCGI;

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
		int				getRequestBodySentCGI() const;
		std::string		*getRequestBody();
		Server			*getRequestServer();
		DirSettings		*getRequestDirSettings();
		std::pair<std::string, std::string> getRequestCGI() const;
		
		void			printDataR();
		void			printSizeR();
		void			printRequestParsedData();

		void			addDataR(char c);
		void 			setRequest(std::vector<Server> *list_server, SocketConnect *socket);
		void 			setRequestHeader();
		void			setRequestContentType();
		bool			checkPort(std::vector<Server>::iterator it, int port);
		void 			findServer();
		void 			findDirSettings();
		void 			checkRedirect();
		void			setRequestBodyLength();
		void			setRequestBody();
		void			checkMethod();
		bool			checkCGI();
		void			addRequestBodySentCGI(int size);


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
		    const char *what() const noexcept;
	};
};

#endif
