/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: lizhang <lizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/03 12:08:06 by lizhang       #+#    #+#                 */
/*   Updated: 2023/07/19 15:58:29 by lizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"

Server::Server()
{
}

Server::Server(std::string settings)
{
	std::string					listen;
	std::vector<std::string>	listensplit;
	std::vector<std::string>	port;
	size_t						pos;

	pos = settings.find("listen", 0);
	listen = getValue(settings, "listen", pos);
	listensplit = charSplit(listen, ' ');
	port = charSplit(listensplit[0], ':');
	Port P1(listensplit[1], atoi(port.back().c_str()));
	this->_ports.push_back(P1);
	pos = listen.length() + 6;
	while (settings.find("listen", pos) != (size_t)(-1))
	{
		pos = settings.find("listen", pos);
		listen = getValue(settings, "listen", pos);
		listensplit = charSplit(listen, ' ');
		port = charSplit(listensplit[0], ':');
		Port P1(listensplit[1], atoi(port.back().c_str()));
		this->_ports.push_back(P1);
		std::cout<<"port number is:"<<port.back()<<std::endl;
		pos = pos + listen.length() + 6;
	}
	std::vector<std::string> set = strSplit(settings, "location");
	std::vector<DirSettings> dir_settings;
	std::cout<<"!!!!"<<std::endl;
	for (unsigned int i = 0; i < set.size(); i++)
	{
		std::cout<<"!!!!"<<std::endl;
		DirSettings D1(set[i]);
		
		if (D1.getDirType() == DEFAULT)
			this->_defDirSettings = D1;
		else if (D1.getDirType() == ROOT)
			this->_rootDirSettings = D1;
		else if (D1.getDirType() == OPTIONAL)
			this->_optDirSettings.push_back(D1);
		else if (D1.getDirType() == CGI)
			this->_cgiDirSettings.push_back(D1);
	}
}

Server::Server(Server const &source)
{
	this->_serverName = source._serverName;
	this->_ports = source._ports;
	this->_defDirSettings = source._defDirSettings;
	this->_rootDirSettings = source._rootDirSettings;
	this->_optDirSettings = source._optDirSettings;
	this->_cgiDirSettings = source._cgiDirSettings;
	//this->_listenSockets = source._listenSockets;
}

Server::~Server()
{
}

Server &Server::operator=(Server const &source)
{
	this->_serverName = source._serverName;
	this->_ports = source._ports;
	this->_defDirSettings = source._defDirSettings;
	this->_rootDirSettings = source._rootDirSettings;
	this->_optDirSettings = source._optDirSettings;
	this->_cgiDirSettings = source._cgiDirSettings;
	//this->_listenSockets = source._listenSockets;
	return (*this);
}

std::string	Server::getServerName() const
{
	return(this->_serverName);
}

// bool	Server::hasSockets() const
// {
// 	if (this->_lisSockets.length() == 0)
// 		return(0);
// 	return (1);
// }

DirSettings Server::getDefDirSettings() const
{
	return (this->_defDirSettings);
}

DirSettings	Server::getRootDirSettings() const
{
	return(this->_rootDirSettings);
}

std::vector<DirSettings> Server::getOptDirSettings() const
{
	return(this->_optDirSettings);
}

std::vector<DirSettings> Server::getCGIDirSettings() const
{
	return(this->_cgiDirSettings);
}

// std::vector<SocketListen>	Server::getListeningSockets() const
// {
// 	if (this->hasSockets() == 0)
// 		throw(std::invalid_argument("Server has no sockets."));
// 	return(this->_listenSockets);
// }

// void Server::addListenSocks(SockListen sock)
// {
// 	this->lisSocks.push_back(sock);
// }