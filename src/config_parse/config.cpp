/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Config.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: lizhang <lizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/26 16:49:22 by lizhang       #+#    #+#                 */
/*   Updated: 2023/07/25 14:53:15 by lizhang       ########   odam.nl         */
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
    std::string	server_info;
	std::string file_content;
	size_t		start_pos = 0;

	file_content = this->_readConfigFile(file_name);
	server_info = getContent(file_content, "server", start_pos);
	Server		newServer(server_info);
	this->_servers.push_back(newServer);
}

//there are multiple servers so a loop to read all servers till there are no more

Config::~Config()
{
}

std::vector<Server> Config::getServers() const
{
	return (this->_servers);
}
