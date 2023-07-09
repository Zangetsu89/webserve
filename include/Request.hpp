#ifndef REQUEST_HPP
# define REQUEST_HPP
# include <vector>
# include <iostream>
# include <sys/socket.h>
# include <sys/types.h>
# include <sys/event.h>
# include <sys/time.h>
# include <stdlib.h>
# include "macro.hpp"
# include "Server.hpp"

class Server;
class SocketConnect;
class DirSettings;
class Request
{
	friend 			SocketConnect;

	// private member
	private:
	std::string		_requestMethod; // bit flag to store 3 method (GET, POST, DELETE) 
	std::string		_requestLocation;
	std::string		_requestHost;
	std::string		_requestPort;
	std::string		_requestBeforeContentLength;
	int				_contentLength;
	std::string		_contentBody;
	DirSettings		*_serverLocation;

	// base member function
	public:
	Request();
	~Request();
	Request& operator=(const Request &source);
	Request(const Request &source);

	// getter
	std::string		getRequestMethod();
	std::string		getRequestLocation();
	std::string		getRequestHost();
	int				getContentLength();

	// checker
	void			checkMethod(std::vector<Server> list_Servers);

};


#endif
