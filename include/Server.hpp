#ifndef SERVER_HPP
# define SERVER_HPP
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
# include "SockConnect.hpp"
# include "SockListen.hpp"

class SockConnect;
class SockListen;
class Server
{
	// private member
	private:
	std::string				_servername;
	std::string				_root_dir;
	std::vector<SockListen>	_listen_socks;

	// base public member function
	public:
	Server();	// do not use : Server must be created with port, name, root_dir, main kqueue
	Server(std::vector<int> ports, std::string name, std::string root_dir, int kq);
	~Server();
	Server& operator=(const Server &source);
	Server(const Server &source);

	// getter
	std::string				GetServername();
	std::string				GetRootDir();
	std::vector<SockListen>	*GetListeningSocks();
	int						CheckIfListeningSock(int sock);


	// exception
	public:
	class	ERR_Server : public std::exception
	{
		private:
			const char	*_error_msg;
		public:
		ERR_Server();
		ERR_Server(const char *error_msg);
		const char *what() const _NOEXCEPT;	// _NOEXCEPT is needed since C++11
	};
};


#endif
