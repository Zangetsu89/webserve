/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Config.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: lizhang <lizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/26 16:49:22 by lizhang       #+#    #+#                 */
/*   Updated: 2023/06/29 16:17:02 by lizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Config.hpp"
#include <iostream>
#include <stdlib.h>


std::string Config::readConfigFile(std::string name)
{
	std::ifstream	file;
	std::string		file_content;

	file.open(file_name);
	if(file.is_open() == 0)
		throw(std::invalid_argument(strerror(errno)));
	getline(file, file_content, '\0');
	if (file_content.length() <= 1)
		throw(std::invalid_argument("No content in config file"));
	file.close();
	return(file_content);
}

Server	newServer(std::string settings)
{
	std::string 				server_name;
	DirSettings					root;
	std::vector<int>			ports;
	std::vector<DirSettings>	optional;

	server_name = getValue(settings, "server_name");
	std::string 				listen = getValue(settings, "listen"));
	std::vector<std::string>	listenSplit = charSplit(listen, ' ');
	for (int i = 0; i < listenSplit.size(); i++)
	{
		std::vector<std::string> ipSplit = charSplit(listenSplit[i], ':');
		ports.insert(ports.end(), atoi(ipSplit.back()));
	}
	server_name = getValue(settings, "server_name");
	this->index = get_value(server_info, "index");
	Server New(server_name, ports, root, optional);
	return(New);
}

Config::Config(std::string file_name)
{
    std::string	server_info;
	std::string file_content;
	size_t		start_pos = 0;

	file_content = this->readConfigFile(file_name);
	while (file_content.find("server", start_pos) != (size_t)(-1))
	{
		server_info = getcontent(file_content, "server", start_pos);
		this->servers.insert(this->newServer(server_info));
		start_pos = start_pos + file_content.find("server", start_pos)
	}
//	this->server_name = get_value(server_info, "server_name");
//	listen = get_value(server_info, "listen");
//	this->listen_address = strtok(strdup(listen.c_str()), " ");
//	this->listen_name = strtok(strdup(listen.c_str()), ";");
//	this->root_location = get_value(server_info, "root");
//	this->index = get_value(server_info, "index");
}

//there are multiple servers so a loop to read all servers till there are no more

Config_mac::~Config_mac()
{
}

std::vector<Server> Config::getServers() const
{
	return (this->servers);
}

Server	Config::get_ServerAtIndex(size_t index) const
{
	return (this->servers[index]);
}