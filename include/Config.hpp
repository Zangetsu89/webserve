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
		void					_printConfigFile();

	public:
			
		Config(std::string file_name);
		virtual ~Config();
		Config() = delete;
		Config	&operator=(const Config &source) = delete;
		Config(Config &source) = delete;

		std::vector<Server>		*getServers();
		void					setKqServers(int kq);
};

#endif
