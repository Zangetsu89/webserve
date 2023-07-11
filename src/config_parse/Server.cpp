/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: lizhang <lizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/03 12:08:06 by lizhang       #+#    #+#                 */
/*   Updated: 2023/07/11 18:11:04 by lizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"

Server::Server()
{
}

Server::Server(std::string server_name, std::vector<int> ports, DirSettings root, \
	std::vector<DirSettings> optional, int kq)
{
	this->_serverName = server_name;
	for (int i = 0; i < ports.size(); i++)
	{
		SockListen newSock(ports[i], kq);
		this->listenSocks.push_back(newSock);
	}
	this->_rootDirSettings = root;
	this->_optDirSettings = optional;
}

Server::Server(const Server &source)
{
	this->_serverName = source.getServerName();
	this->_rootDirSettings = source.getRootDirSettings();
	this->_optDirSettings = source.getOptDirSettings();
	this->_lisSocks = source.getListeningSocks();
}

Server::~Server()
{
}

Sever &Server::operator=(const Server &source)
{
	this->_serverName = source.getServerName();
	this->_rootDirSettings = source.getRootDirSettings();
	this->_optDirSettings = source.getOptDirSettings();
	this->_lisSocks = source.getListeningSocks();
	return (*this);
}

std::string	Server::getServerName() const
{
	return(this->_serverName);
}

DirSettings	Server::getRootDirSettings() const
{
	return(this->_rootDirSettings);
}

std::vector<DirSettings> Server::getOptDirSettings() const
{
	return(this->_optDirSettings);
}

std::vector<SockListen>	Server::getListeningSocks() const
{
	return(this->lisSocks);
}

void Server::addListenSocks(SockListen sock)
{
	this->lisSocks.push_back(sock);
}