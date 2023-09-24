/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ConfigChecker.cpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: lizhang <lizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/08 14:05:16 by lizhang       #+#    #+#                 */
/*   Updated: 2023/08/08 14:41:48 by lizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ConfigChecker.hpp"

//throw error if there is unmatching data
//no servers;
//for servers:
//no listening port
//no index file
//no server_name
//no root directory
//wrong allowed_methods

void	checkDirMethods(DirSettings D1, unsigned int server_number)
{
	std::vector<std::string> methods = D1.getMethods();
	for (unsigned int j = 0; j < methods.size(); j++)
		if (methods[j]!= "GET" && methods[j]!= "POST" && methods[j]!= "DELETE")
			throw(Exception_StopServer("Server " + std::to_string(server_number) + " contains wrong allowed_methods."));	
}

void    checkConfigFile(Config C1)
{
	if (C1.getServers().size() < 1)
		throw (Exception_StopServer("No servers information info can be read from Config file"));
	std::vector<Server> Servers = C1.getServers();
	for (unsigned int i = 0; i < Servers.size(); i++)
	{
		if (Servers[i].getServerName().length() < 1)
			throw(Exception_StopServer("Server " + std::to_string(i) + " has no name"));
		if (Servers[i].getPorts().size() < 1)
			throw(Exception_StopServer("Server " + std::to_string(i) + " has no listening ports"));
		if (Servers[i].getRootDir().length() < 1)
			throw(Exception_StopServer("Server " + std::to_string(i) + " has no root directory path"));
		if (Servers[i].getRootDirSettings().getIndexPage().length() < 1)
			throw(Exception_StopServer("Server " + std::to_string(i) + " has no index page"));
		checkDirMethods(Servers[i].getRootDirSettings(), i);
		for (unsigned int j = 0; j < Servers[i].getOptDirSettings().size(); j++)
			checkDirMethods(Servers[i].getOptDirSettings()[j], i);
		for (unsigned int j = 0; j < Servers[i].getCGIDirSettings().size(); j++)
			checkDirMethods(Servers[i].getCGIDirSettings()[j], i);
	}
}