//
// Server
// Server class stores server information and set listening socket(s) belonging to this server
//

#include "../../include/Server.hpp"

Server::Server(std::string server_name, std::string rootDir, std::vector<int> ports)
{
	this->_serverName = server_name; 
	this->_rootDir = rootDir; 
	// this->_rootDirSettings = root; 
	// this->_optDirSettings = optional; 
	this->_listPort = ports;
}

Server::~Server()
{

}

Server& Server::operator=(const Server &source)
{
	if (this != &source)
	{
		_serverName = source._serverName;
		_rootDir = source._rootDir;
		_rootDirSettings = source._rootDirSettings;
		_optDirSettings = source._optDirSettings;
		_listPort = source._listPort;
		_listSocketListen = source._listSocketListen;
	}
	return (*this);
}

Server::Server(const Server &source)
{
	*this = source;
}

// getter

std::string		Server::getServerName()
{
	return (_serverName);
}

DirSettings	*Server::getRootDirSettings() 
{
	return(&_rootDirSettings);
}

std::vector<DirSettings> *Server::getOptDirSettings()
{
	return(&_optDirSettings);
}

std::string		Server::getRootDir()
{
	return (_rootDir);
}

std::vector<SocketListen>	*Server::getListeningSocket()
{
	return (&_listSocketListen);
}

int	Server::checkListeningSocket(int sock)
{
	for (int i = 0; i < (int)_listSocketListen.size(); i++)
	{
		if (_listSocketListen[i].getSocketListen() == sock)
			return (sock);
	}
	return (-1);
}

void	Server::setSocketListen(int kq)
{
	for (int i = 0; i < (int)_listPort.size(); i++)
	{
		SocketListen tempsock(_listPort[i], kq);
		_listSocketListen.push_back(tempsock);
		// SocketListen	*tempsock = new SocketListen(_listPort[i], kq);
		// _listSocketListen.push_back(*tempsock);
		// delete (tempsock);
	}
}

void	Server::setDirSettings(DirSettings rootDir, std::vector<DirSettings> optDir)
{
	_rootDirSettings = rootDir;
	_optDirSettings = optDir;
}

// exception
Server::ERR_Server::ERR_Server():_error_msg("server setting failed"){}
Server::ERR_Server::ERR_Server(const char *error_msg):_error_msg(error_msg){}

const char	*Server::ERR_Server::what() const _NOEXCEPT
{
    return (_error_msg);
}
