#ifndef SOCKETCONNECT_HPP
# define SOCKETCONNECT_HPP
# include <vector>
# include <iostream>
# include <fstream>
# include <sstream>
# include <sys/socket.h>
# include <sys/types.h>
# include <sys/event.h>
# include <sys/time.h>
# include <stdlib.h>
# include <unistd.h>
# include <netdb.h>
# include <signal.h>
# include <fcntl.h>
# include "Response.hpp"
# include "Server.hpp"
# include "Request.hpp"


class Server;
class Request;
class Response;
class SocketConnect
{

	private:
	int					_numSocket;
	struct sockaddr_in	_clientSockaddr;
	socklen_t 			_clientSockaddrLen;
	struct kevent		_clientKevent;
	std::vector<Server>	*_servers;
    Request				_clientRequest;
	Response			_clientResponse;
	int					_errorNum;
	int					_statusNum;
	std::string			_redirectURL;
	struct timeval		_timeout;

	public:
	SocketConnect();
	SocketConnect(int socket, int kq, std::vector<Server> *servers);
	~SocketConnect();
	SocketConnect& operator=(const SocketConnect &source);
	SocketConnect(const SocketConnect &source);
	

	// getter
	int 				getSocketConnect();
	Request				*getClientRequest();
	Response			*getClientResponse();
	int					getErrorNum();
	int					getStatusNum();
    int                 getNumSocket();
    std::string         getRedirectURL();


	// setter and others
	int					readRequest();
	void                setRequest(std::vector<Server> *list_server);
	void				setError(int err);
	void				setStatus(int status);
	void				setRedirect(std::string url);

	// exception
	public : class ERR_SocketConnect : public std::exception
	{
		private:
			const char	*_error_msg;
		public:
		ERR_SocketConnect();
		ERR_SocketConnect(const char *error_msg);
		const char *what() const _NOEXCEPT;	// _NOEXCEPT is needed since C++11
	};
};

#endif
