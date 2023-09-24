#ifndef REQUESTHEADER_HPP
# define REQUESTHEADER_HPP
# include <vector>
# include <map>
# include <iostream>
# include <sys/socket.h>
# include <sys/event.h>
# include "macro.hpp"

class Server;
class SocketConnect;
class DirSettings;
class RequestHeader
{
	private:
		std::string _requestHeaderMethod;			// GET, POST, DELETE
		std::string _requestHeaderLocation; 		// the whole location string (ex: "/hello/world/filename" )
		std::string _requestHeaderLocationParameter; // Information by Get method
		std::string _requestHeaderHTTP_Protocol;
		std::string _requestHeaderHost;
		std::string _requestHeaderPort;
		std::map<std::string, std::string> _requestHeaderOthers;

		SocketConnect			*_requestSocket;
		std::vector<Server>		*_servers;

	public:
		RequestHeader();
		~RequestHeader();
		RequestHeader& operator=(const RequestHeader &source);
		RequestHeader(const RequestHeader &source);

		std::string		getRequestMethod() const;
		std::string		getRequestLocation() const;
		std::string		getRequestLocationParametor() const;
		std::string		getHTTPProtocol() const;
		std::string		getRequestHost() const;
		std::string		getRequestPort() const;
		std::map<std::string, std::string> *getHeaderOthers();

		void			displayHeaderOthers();
		void			displayHeaderAll();

		void			checkLocationParametor();
		int				setMethodLocationProtocol(std::vector<char> *dataR);
		int				setHeaderOthers(std::vector<char> *dataR);
		int				setHostPort();
		int				setContentType();
		int				setRequestContentLength();

};

#endif
