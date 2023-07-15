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
	std::string		_requestMethod; 			// GET, POST, DELETE
	std::string		_requestLocation;			// the whole location string (ex: "/hello/world/filename" )
	std::string		_requestFile;				// the element after the last "/" (ex: "filename"), if location ends with "/", the default index file comes here
	std::string		_requestHTTPprotocol;
	std::string		_requestHost;
	std::string		_requestPort;
	std::map<std::string, std::string>	_requestHeaderOthers;
	int				_contentLength;
	std::string		_contentBody;
	Server			*_requestServer;
	DirSettings		*_requestDirSetting;

	// base member function
	public:
	Request();
	~Request();
	Request& operator=(const Request &source);
	Request(const Request &source);

	// getter
	std::string		getRequestMethod();
	std::string		getRequestLocation();
	std::vector<std::string>	getRequestDir();
	std::string		getRequestHost();
	std::string		getRequestPort();
	int				getContentLength();
	void			displayHeaderOthers();
	void			displayHeaderAll();

	// setter
	void			setHeaderOthers(std::pair<std::string, std::string> datapair);
	void			setFile();
	void			setHostPortLength();
	// checker
	bool			checkMethod();

};


#endif
