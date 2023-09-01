#ifndef REQUESTHEADER_HPP
# define REQUESTHEADER_HPP
# include <vector>
# include <map>
# include <iostream>
# include <string>
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
class RequestHeader
{
	private:
	std::string _requestMethod;	  // GET, POST, DELETE
	std::string _requestLocation; // the whole location string (ex: "/hello/world/filename" )
	std::string _requestHTTPprotocol;
	std::string _requestHost;
	std::string _requestPort;
	std::string _requestContentType;
	int			_requestContentLength;
	std::map<std::string, std::string> _requestHeaderOthers;

	SocketConnect			*_requestSocket;
	std::vector<Server>		*_servers;

	public:
	RequestHeader();
	~RequestHeader();
	RequestHeader& operator=(const RequestHeader &source);
	RequestHeader(const RequestHeader &source);

	// getter
	std::string		getRequestMethod();
	std::string		getRequestLocation();
	std::string		getHTTPProtocol();
	std::string		getRequestHost();
	std::string		getRequestPort();
	std::map<std::string, std::string> *getHeaderOthers();

	void			displayHeaderOthers();
	void			displayHeaderAll();

	// setter
	int				setMethodLocationProtocol(std::vector<char> *dataR);
	int				setHeaderOthers(std::vector<char> *dataR);
	int				setHostPort();
	int				setContentType();
	int				setRequestContentLength();

};

#endif
