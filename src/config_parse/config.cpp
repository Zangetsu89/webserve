/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   config.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: lizhang <lizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/26 16:49:22 by lizhang       #+#    #+#                 */
/*   Updated: 2023/06/29 16:17:02 by lizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/config.hpp"
#include <fstream>
#include <iostream>

static std::string getcontent(std::string file_content, std::string title)
{
	size_t	pos_start;
	size_t	pos_end;

	pos_start = file_content.find(title, 0);
	pos_start = pos_start + title.length();
	pos_start = file_content.find("{", pos_start);
	pos_start ++;
	pos_end = file_content.find("}", pos_end);
	return(file_content.substr(pos_start, pos_end));
}

static std::string get_value(std::string content, std::string title)
{
	std::string line;
	size_t		pos = 0;
	size_t		pos2;

	while (pos < content.length())
	{
		pos2 = content.find(";", pos);
		line = content.substr(pos, pos2 - pos);
		if (line.find(title) < line.length())
			break ;
		pos = pos2 + 1;
	}
	pos = line.find(" ");
	return (line.substr(pos + 1, line.length() - pos - 1));
}

Config_mac::Config_mac(std::string file_name)
{
    std::ifstream	file;
	std::string	file_content;
	std::string	server_info;
	std::string listen;

    file.open(file_name);
    if(file.is_open() == 0)
		throw(std::invalid_argument(strerror(errno)));
	getline(file, file_content, '\0');
	if (file_content.length() <= 1)
		throw(std::invalid_argument("No content in config file"));
	file.close();
	server_info = getcontent(file_content, "server");
	this->server_name = get_value(server_info, "server_name");
	listen = get_value(server_info, "listen");
	this->listen_address = strtok(strdup(listen.c_str()), " ");
	this->listen_name = strtok(strdup(listen.c_str()), ";");
	this->root_location = get_value(server_info, "root");
	this->index = get_value(server_info, "index");
}

Config_mac::~Config_mac()
{

}

std::string Config_mac::get_server_name()
{
	return (this->server_name);
}

std::string	Config_mac::get_port()
{
	return (this->listen_address);
}

std::string	Config_mac::get_port_name()
{
	return (this->listen_name);
}

std::string	Config_mac::get_root()
{
	return (this->root_location);
}

std::string	Config_mac::get_index()
{
	return (this->index);
}