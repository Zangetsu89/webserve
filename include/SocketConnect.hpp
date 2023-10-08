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
# include "macro.hpp"
# include "Response.hpp"
# include "Server.hpp"
# include "Request.hpp"
# include "CgiHandler.hpp"


class Server;
class Request;
class Response;
class SocketConnect
{

	private:
		int					_numSocket;
		int					_kqueueNum;
		struct sockaddr_in	_socketAddr;
		socklen_t 			_socketAddrLen;
		struct kevent		_socketKevent;
		std::vector<Server>	*_servers;
		std::vector<SocketConnect *> *_socketsList;
    	Request				_socketRequest;
		Response			_socketResponse;
		CgiHandler			_socketCgiHandler;

	public:
		SocketConnect();
		SocketConnect(int socket, int kq, std::vector<Server> *servers, std::vector<SocketConnect *> *list);
		~SocketConnect();
		SocketConnect& operator=(const SocketConnect &source);
		SocketConnect(const SocketConnect &source);
	
    	int				getNumSocket() const;
    	int				getKqueueNum() const;
    	struct kevent	*getSocketKevent();
		Request			*getSocketRequest();
		Response		*getSocketResponse();
		CgiHandler		*getsocketCgiHandler();
		void			setKevent_READ();
		void			setKevent_WRITE();
		int				readRequest();
		int 			readResponseFile();
		void			setRequest(std::vector<Server> *list_server);
		bool			doRedirect(std::vector<SocketConnect*> registerdSockets, int where);

	// exception
	public : class ERR_SocketConnect : public std::exception
	{
		private:
			const char	*_error_msg;
		public:
			ERR_SocketConnect();
			ERR_SocketConnect(const char *error_msg);
			const char *what() const noexcept;
	};
};

#endif
