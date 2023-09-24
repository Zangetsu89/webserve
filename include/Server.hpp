#ifndef SERVER_HPP
# define SERVER_HPP
# include <iostream>
# include <vector>
# include <string>
# include "DirSettings.hpp"

class SocketListen;
class DirSettings;
class Server
{
	private:
		std::string						_serverName;
		std::vector<int>				_ports;
		std::string						_rootPath;
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
		std::string					getRootPath() const;
		DirSettings					*getRootDirSettings();
		std::vector<DirSettings>	*getOptDirSettings();
		std::vector<DirSettings>	*getCGIDirSettings();
		std::vector<SocketListen>	*getSocketListen();

		void						setServerName(std::string *settings, std::string title);
		void						setRootPath(std::string *settings, std::string title);
		void						setPorts(std::string *settings, std::string title);
		bool						checkServerInfo();
		void						setSocketListen(int kq);
    	bool                        isNumeric(std::string str);
};


#endif
