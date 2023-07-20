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
# include "DirSettings.hpp"
# include "ConfigMacros.hpp"

class SocketListen;
class DirSettings;
class Server
{
	// private member
	private:
	std::string					_serverName;
	std::string					_rootDir;
	DirSettings					_rootDirSettings;
	std::vector<DirSettings>	_optDirSettings;
	std::vector<int>			_listPort;
	std::vector<SocketListen>	_listSocketListen;

	// base member function
	protected:
	Server();	// do not use : Server must be created with port(s), name, root_dir, main kqueue

	public:
	Server(std::string server_name, std::string rootDir, std::vector<int> ports);
	~Server();
	Server& operator=(const Server &source);
	Server(const Server &source);

	// getter
	std::string					getServerName();
	DirSettings					*getRootDirSettings() ;
	std::vector<DirSettings>	*getOptDirSettings();
	std::string					getRootDir();
	std::vector<SocketListen>	*getListeningSocket();
	int							checkListeningSocket(int sock);

	// setter
	void						setSocketListen(int kq);
	void						setDirSettings(DirSettings rootDir, std::vector<DirSettings> optDir);

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
