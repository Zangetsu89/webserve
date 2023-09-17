#include "../include/Config.hpp"
#include "../include/ConfigMacros.hpp"

std::string Config::_readConfigFile(std::string name)
{
	std::ifstream file(name);
	std::string file_content;

	if (!file.is_open())
		throw(std::invalid_argument(strerror(errno)));
	getline(file, file_content, '\0');
	if (file_content.length() <= 1)
		throw(std::invalid_argument("No content in config file"));
	file.close();
	return (file_content);
}

Config::Config(std::string file_name)
{
	std::vector<std::string> server_info;
	std::string file_content;

	file_content = this->_readConfigFile(file_name);
	try
	{
		try
		{
			server_info = getAllOf(file_content, "server ");
		}
		catch (std::exception &e)
		{
			throw std::invalid_argument("No server block in config file");
		}
		for (unsigned int i = 0; i < server_info.size(); i++)
		{
			Server S1(server_info[i]);
			this->_servers.push_back(S1);
		}
		for (size_t i = 0; i < _servers.size(); i++)
		{
			std::cout << std::endl;
			try
			{
				std::cout << i << ": server name is " << _servers[i].getServerName() << std::endl;
			}
			catch (std::exception &e)
			{
				std::cout << e.what() << std::endl;
			}
			try
			{
				std::cout << "rootdir is " << _servers[i].getRootDir() << std::endl;
			}
			catch (std::exception &e)
			{
				std::cout << e.what() << std::endl;
			}
			try
			{
				for (size_t j = 0; j < _servers[i].getPorts().size(); j++)
				{
					std::cout << "port: " << _servers[i].getPorts()[j] << std::endl;
				}
			}
			catch (std::exception &e)
			{
				std::cout << e.what() << std::endl;
			}
			try
			{
				_servers[i].getRootDirSettings()->printAllDirSettings();
			}
			catch (std::exception &e)
			{
				std::cout << e.what() << std::endl;
			}
			try
			{
				for (size_t j = 0; j < _servers[i].getOptDirSettings()->size(); j++)
				{
					std::cout << std::endl
							  << "[optdir " << j << "]\n";
					std::vector<DirSettings>::iterator itr = _servers[i].getOptDirSettings()->begin();
					// itr += j;
					advance(itr, j);
					itr->printAllDirSettings();
				}
			}
			catch (std::exception &e)
			{
				std::cout << e.what() << std::endl;
			}
			try
			{
				for (size_t j = 0; j < _servers[i].getCGIDirSettings()->size(); j++)
				{
					std::cout << std::endl
							  << "[CGIdir " << j << "]\n";
					std::vector<DirSettings>::iterator itr = _servers[i].getCGIDirSettings()->begin();
					// itr += j;
					advance(itr, j);
					itr->printAllDirSettings();
				}
			}
			catch (std::exception &e)
			{
				std::cout << e.what() << std::endl;
			}
			std::cout << std::endl;
		}
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
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

void Config::printAll()
{
	// std::cout << "_servers.size() is " << _servers.size() << std::endl;
	for (size_t i = 0; i < _servers.size(); i++)
	{
		std::cout << "server name is " << _servers[i].getServerName() << std::endl;
		std::cout << "rootdir is " << _servers[i].getRootDir() << std::endl;
		_servers[i].getRootDirSettings()->printAllDirSettings();
		// not yet done... add all element
	}
}
