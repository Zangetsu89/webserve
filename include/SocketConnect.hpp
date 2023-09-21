#ifndef SOCKETCONNECT_HPP
# define SOCKETCONNECT_HPP
# include <vector>
# include <iostream>
# include <fstream>
# include <sys/socket.h>
# include <sys/types.h>
# include <sys/event.h>
# include <sys/time.h>
# include <unistd.h>
# include <netdb.h>
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
		struct timeval		_timeout;

	public:
		SocketConnect();
		SocketConnect(int socket, int kq, std::vector<Server> *servers);
		~SocketConnect();
		SocketConnect& operator=(const SocketConnect &source);
		SocketConnect(const SocketConnect &source);
	
    	int				getNumSocket() const;
		Request			*getClientRequest();
		Response		*getClientResponse();
		int				getErrorNum() const;
		int				getStatusNum() const;

		void			setError(int err);
		void			setStatus(int status);
		int				readRequest();
		void			setRequest(std::vector<Server> *list_server);
		bool			doRedirect(std::vector<SocketConnect*> socketConnects, int where);

	// exception
	public : class ERR_SocketConnect : public std::exception
	{
		private:
			const char	*_error_msg;
		public:
			ERR_SocketConnect();
			ERR_SocketConnect(const char *error_msg);
			const char *what() const _NOEXCEPT;
	};
};

#endif
