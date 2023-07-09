#ifndef SERVER_HPP
# define SERVER_HPP
# include <vector>
# include <iostream>
# include <sys/socket.h>
# include <sys/types.h>
# include <sys/event.h>
# include <sys/time.h>
# include <stdlib.h>
# include "macro.hpp"
# include "SocketListen.hpp"

class SocketListen;
class Server
{
	// private member
	private:
	std::string					_serverName;
	std::string					_rootDir;
	std::vector<int>			_listPort;
	std::vector<SocketListen>	_listSocketListen;

	// base member function
	protected:
	Server();	// do not use : Server must be created with port(s), name, root_dir, main kqueue

	public:
	Server(std::vector<int> ports, std::string name, std::string root_dir, int kq);
	~Server();
	Server& operator=(const Server &source);
	Server(const Server &source);

	// getter
	std::string					getServerName();
	std::string					getRootDir();
	std::vector<SocketListen>	*getListeningSocket();
	int							checkListeningSock(int sock);

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
