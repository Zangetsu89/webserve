/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Config.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: lizhang <lizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/26 16:49:22 by lizhang       #+#    #+#                 */
/*   Updated: 2023/08/07 13:35:33 by keika         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Config.hpp"

std::string Config::_readConfigFile(std::string name)
{
	std::ifstream	file(name);
	std::string		file_content;

	if(!file.is_open())
		throw(std::invalid_argument(strerror(errno)));
	getline(file, file_content, '\0');
	if (file_content.length() <= 1)
		throw(std::invalid_argument("No content in config file"));
	file.close();
	return(file_content);
}


Config::Config(std::string file_name)
{
    std::vector<std::string>	server_info;
	std::string					file_content;

	file_content = this->_readConfigFile(file_name);
	server_info = getAllOf(file_content, "server ");
	for (unsigned int i = 0; i < server_info.size(); i++)
	{
		Server S1(server_info[i]);
		this->_servers.push_back(S1);
	}
		for (size_t i = 0; i < _servers.size(); i++)
	{
		std::cout << std::endl;
		std::cout << i << ": server name is " << _servers[i].getServerName() << std::endl;
		std::cout << "rootdir is " << _servers[i].getRootDir() << std::endl;
		for (size_t j = 0; j < _servers[i].getPorts().size(); j++)
		{
			std::cout << "port: " << _servers[i].getPorts()[j] << std::endl;
		}
		_servers[i].getRootDirSettings()->printAllDirSettings();
		for (size_t j = 0; j < _servers[i].getOptDirSettings()->size(); j++)
		{
			std::cout << "[optdir " << j << "]\n";
			std::vector<DirSettings>::iterator itr = _servers[i].getOptDirSettings()->begin();
			// itr += j; 
			advance(itr, j);
			itr->printAllDirSettings();
		}
		std::cout << std::endl;
	}
}

Config::~Config()
{
}

std::vector<Server> *Config::getServers()
{
	return (&this->_servers);
}

void	Config::setKqServers(int kq)
{
	for (size_t i = 0; i < _servers.size(); i++)
	{
		_servers[i].setSocketListen(kq);
	}
}


void	Config::printAll()
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
