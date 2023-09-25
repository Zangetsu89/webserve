#ifndef SOCKETLISTEN_HPP
# define SOCKETLISTEN_HPP
# include <vector>
# include <iostream>
# include <sys/event.h>
# include <netdb.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <sys/time.h>
# include <fcntl.h>

class Server;
class SocketListen
{
	private:
		int					_numSocket;
		struct sockaddr_in	_listenSockaddr;
		socklen_t 			_listenSockaddrLen;
		struct kevent		_listenKevent;
		struct timeval		_timeout;

	public:
		SocketListen(int port, int kq);
		~SocketListen();
		SocketListen() = delete;
		SocketListen& operator=(const SocketListen &source);
		SocketListen(const SocketListen &source);

		int 			getNumSocket() const;
		struct kevent	*getKevent();

		class	ERR_SocketListen : public std::exception
		{
			private:
				const char	*_error_msg;
			public:
				ERR_SocketListen();
				ERR_SocketListen(const char *error_msg);
				const char *what() const noexcept;
		};
};

#endif
