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
class SocketConnect
{
	// private member
	private:
	int					_numSocket;
	struct sockaddr_in	_clientSockaddr;
	socklen_t 			_clientSockaddrLen;
	struct kevent		_connectKevent;
	
	std::vector<char>	_dataR;
	std::vector<char>	_dataW;
	int					_sizeR;
	int					_sizeW;

	// base member function
	protected:
	SocketConnect();	// do not use : SocketConnect must be created with socket number, associated server, kq

	public:
	SocketConnect(int socket, int kq);
	~SocketConnect();
	SocketConnect& operator=(const SocketConnect &source);
	SocketConnect(const SocketConnect &source);
	

	// getter and changer
	int 			getSocketConnect();
	struct kevent	*getKevent();
	void			addReadData(char *buff, int size);
	void			printReadData();
	int				getSizeR();
	int				getSizeW();
	std::vector<char>	*getDataR();
	std::vector<char>	*getDataW();

	// exception
	public:
	class	ERR_SocketConnect : public std::exception
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
