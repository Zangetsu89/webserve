#ifndef SOCKETCONNECT_HPP
# define SOCKETCONNECT_HPP
# include <vector>
# include <iostream>
# include <sys/socket.h>
# include <sys/types.h>
# include <sys/event.h>
# include <sys/time.h>
# include <stdlib.h>
# include <unistd.h>
# include <netdb.h>
# include <signal.h>
# include <fcntl.h>
# include "Server.hpp"
# include "Request.hpp"

class Server;
//class Request;
class SocketConnect
{

	private:
	int					_numSocket;
	struct sockaddr_in	_clientSockaddr;
	socklen_t 			_clientSockaddrLen;
	struct kevent		_clientKevent;
	std::vector<Server>	*_servers;
    Request             *_clientRequest;
	// Response			_clientResponse;
	int					_errorNum;
	// Error				_errorInfo;

	public:
	SocketConnect();
	SocketConnect(int socket, int kq, std::vector<Server> *servers);
	~SocketConnect();
	SocketConnect& operator=(const SocketConnect &source);
	SocketConnect(const SocketConnect &source);
	

	// getter
	int 				getSocketConnect();
	Request				*getClientRequest();
	// Response			*getClientResponse();
	int					getErrorNum();
	// Error				*getErrorInfo();


	// setter and others
	int setRequest(std::vector<Server> *list_server);
	void				setError(int err);
	int					sendResponse();

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
