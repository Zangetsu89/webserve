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
# include "Request.hpp"

class Server;
class Request;
class SocketConnect
{
	// private member
	private:
	int					_numSocket;
	struct sockaddr_in	_clientSockaddr;
	socklen_t 			_clientSockaddrLen;
	struct kevent		_clientKevent;
	
	std::vector<char>	_dataR;
	std::string			_dataW;
	int					_sizeR;
	int					_sizeW;
	Request				_clientRequest;
	int					_error;

	// base member function
	protected:
	SocketConnect();	// do not use : SocketConnect must be created with socket number, associated server, kq

	public:
	SocketConnect(int socket, int kq);
	~SocketConnect();
	SocketConnect& operator=(const SocketConnect &source);
	SocketConnect(const SocketConnect &source);
	

	// getter
	int 				getSocketConnect();
	struct kevent		*getKevent();
	int					getSizeR();
	int					getSizeW();
	std::vector<char>	*getDataR();
	char				*getDataW();
	Request				*getClientRequest();
	int					getError();


	// setter and others
	void				addReadData(char *buff, int size);
	void				setDataW(std::string str);
	void				setSizeR(int i);
	void				setSizeW(int i);
	void				printReadData();
	int					setRequest(std::vector<char> *data_read, std::vector<Server> list_server);
	void				setError(int err);

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
