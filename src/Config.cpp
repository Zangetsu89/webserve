#include "../include/Config.hpp"
#include "../include/ConfigMacros.hpp"
#include "../include/util.hpp"

std::string Config::_readConfigFile(std::string name)
{
	std::ifstream	file(name);
	std::string		file_content;

	if (!file.is_open())
		throw Exception_StopServer(strerror(errno));
	getline(file, file_content, '\0');
	file_content = checkConfigString(file_content);
	if (file_content.size() == 0)
		throw Exception_StopServer("Config format is wrong");
	file.close();
	return (file_content);
}

Config::Config(std::string file_name)
{
	std::vector<std::string> server_info;
	std::string file_content;

	file_content = this->_readConfigFile(file_name);
	file_content = "\n" + file_content;
	server_info = getAllOf(file_content, "\nserver ");
	if (server_info.size() == 0)
		throw Exception_StopServer("Invalid config, no server info");
	for (unsigned int i = 0; i < server_info.size(); i++)
	{
		Server S1(server_info[i]);
		this->_servers.push_back(S1);
	}
}

Config::~Config()
{
}

std::vector<Server> *Config::getServers()
{
	return (&this->_servers);
}

void Config::setKqServers(int kq)
{
	for (size_t i = 0; i < _servers.size(); i++)
	{
		_servers[i].setSocketListen(kq);
	}
}

void	Config::printConfigFile()
{
	std::cout << "- - - - - - - - - - - - " << std::endl << std::endl;
	for (size_t i = 0; i < _servers.size(); i++)
	{
		std::cout << i << ": server name is " << _servers[i].getServerName() << std::endl;
		std::cout << "root path is " << _servers[i].getRootPath() << std::endl;
		for (size_t j = 0; j < _servers[i].getPorts().size(); j++)
		{
			std::cout << "port: " << _servers[i].getPorts()[j] << std::endl;
		}
		std::cout << std::endl << "[Rootdir] " << "\n";
		_servers[i].getRootDirSettings()->printAllDirSettings();
		for (size_t j = 0; j < _servers[i].getOptDirSettings()->size(); j++)
		{
			std::cout << std::endl << "[Optdir " << j << "]\n";
			std::vector<DirSettings>::iterator itr = _servers[i].getOptDirSettings()->begin();
			advance(itr, j);
			itr->printAllDirSettings();
		}
		std::cout << std::endl << "- - - - - - - - - - - - " << std::endl << std::endl;
	}

}