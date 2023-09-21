#ifndef SERVER_HPP
# define SERVER_HPP
# include <iostream>
# include <vector>
# include "DirSettings.hpp"

class SocketListen;
class DirSettings;
class Server
{
	private:
		std::string						_serverName;
		std::vector<int>				_ports;
		std::string						_rootDir;
		DirSettings						_rootDirSettings;
		std::vector<DirSettings>		_optDirSettings;
		std::vector<DirSettings>		_cgiDirSettings;
		std::vector<SocketListen>		_listSocketListen;

	public:
		Server();
		Server(std::string settings);
		Server(Server const &source);
		~Server();
		Server	&operator=(Server const &source);

		std::string					getServerName() const;
		std::vector<int>			getPorts() const;
    	std::string					getRootDir() const;
		DirSettings					*getRootDirSettings();
		std::vector<DirSettings>	*getOptDirSettings();
		std::vector<DirSettings>	*getCGIDirSettings();
		std::vector<SocketListen>	*getSocketListen();

		void						setSocketListen(int kq);
    	bool                        isNumeric(std::string str);

	//exception
	public:
	class	ERR_Server : public std::exception
	{
		private:
			const char	*_error_msg;
		public:
			ERR_Server();
			ERR_Server(const char *error_msg);
			const char *what() const _NOEXCEPT;
	};
};


#endif
