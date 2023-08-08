/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Config.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: lizhang <lizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/26 16:49:22 by lizhang       #+#    #+#                 */
/*   Updated: 2023/07/26 13:26:48 by lizhang       ########   odam.nl         */
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
}

Config::~Config()
{
}

std::vector<Server> Config::getServers() const
{
	return (this->_servers);
}
