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

class Server;
class SocketConnect;
class DirSettings;
class Request
{
	friend 			SocketConnect;

	// private member
	private:
	std::string		_requestMethod; // GET, POST, DELETE
	std::string		_requestLocation;
	std::string		_requestHTTPprotocol;
	std::string		_requestHost;
	std::string		_requestPort;
	std::map<std::string, std::string>	_requestHeaderOthers;
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
	std::string		getRequestPort();
	int				getContentLength();
	void			displayHeaderOthers();
	void			displayHeaderAll();

	// setter
	void			setLabelContent(std::pair<std::string, std::string> datapair);
	void			setHostPortLength();
	// checker
	bool			checkMethod(std::vector<Server> list_Servers);

};


#endif
