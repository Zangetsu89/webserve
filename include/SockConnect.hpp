#ifndef SOCKCONNECT_HPP
# define SOCKCONNECT_HPP
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
class SockConnect
{
	// private member
	private:
	Server				*_num_Server;
	int					_num_socket;
	struct sockaddr_in	_client_sockaddr;
	socklen_t 			_client_sockaddr_len;
	struct kevent		_kev_connect;
	
	std::vector<char>	_data_r;
	std::vector<char>	_data_w;
	int					_size_r;
	int					_size_w;

	// base member function
	protected:
	SockConnect();	// do not use : SockConnect must be created with socket number, associated server, kq

	public:
	SockConnect(int socket, Server *server, int kq);
	~SockConnect();
	SockConnect& operator=(const SockConnect &source);
	SockConnect(const SockConnect &source);
	

	// getter and changer
	int 			GetSockConnect();
	struct kevent	*GetKevent();
	void			AddReadData(char *buff, int size);
	void			PrintReadData();

	// exception
	public:
	class	ERR_SockConnect : public std::exception
	{
		private:
			const char	*_error_msg;
		public:
		ERR_SockConnect();
		ERR_SockConnect(const char *error_msg);
		const char *what() const _NOEXCEPT;	// _NOEXCEPT is needed since C++11
	};
};

#endif
