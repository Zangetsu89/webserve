#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <string>
# include <fstream>
# include "Server.hpp"

class Config
{
	private:
		std::vector<Server>		_servers;
		std::string				_readConfigFile(std::string name);

	public:
		Config(std::string file_name);
		~Config();
		std::vector<Server>		*getServers();
		void					setKqServers(int kq);
		void					printAll();
};

#endif
