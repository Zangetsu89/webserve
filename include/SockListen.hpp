#ifndef SOCKLISTEN_HPP
# define SOCKLISTEN_HPP
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

class Server;
class SockListen
{
	// private member
	private:
	Server				*_num_Server;
	int					_num_socket;
	struct sockaddr_in	_listen_sockaddr;
	socklen_t 			_listen_sockaddr_len;
	struct kevent		_kev_listen;
	

	// base public member function
	public:
	SockListen();				// do not use : SockListen must be created with socket number
	SockListen(int port, Server *server, int kq);
	~SockListen();
	SockListen& operator=(const SockListen &source);
	SockListen(const SockListen &source);
	

	// getter and changer
	int 			GetSockListen();
	struct kevent	*GetKevent();
	void			AddReadData(char *buff, int size);
	void			PrintReadData();

	// exception
	public:
	class	ERR_SockListen : public std::exception
	{
		private:
			const char	*_error_msg;
		public:
		ERR_SockListen();
		ERR_SockListen(const char *error_msg);
		const char *what() const _NOEXCEPT;	// _NOEXCEPT is needed since C++11
	};
};

#endif
