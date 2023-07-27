//
// Server
// Server class stores server information and set listening socket(s) belonging to this server
//

#include "../../include/Server.hpp"
#include "../../include/macro.hpp"


Server::Server(std::vector<int> ports, std::string name, std::string root_dir, int kq):
_serverName(name), _rootDir(root_dir),_listPort(ports)
{
	if (name == "" || root_dir == "")
		throw ERR_Server("Server setting info is wrong");
	
	for (int i = 0; i < (int)ports.size(); i++)
	{
		SocketListen	*tempsock = new SocketListen(ports[i], kq);
		_listSocketListen.push_back(*tempsock);
		delete(tempsock);
		std::cout << ports[i] << " port converts to socket " << _listSocketListen.back().getSocketListen() << std::endl;
	}
	std::cout << _listSocketListen.size() << " listening sockets are made!" << std::endl;
}

Server::~Server()
{

}

Server& Server::operator=(const Server &source)
{
	_serverName = source._serverName;
	_rootDir = source._rootDir;
	_listPort = source._listPort;
	_listSocketListen = source._listSocketListen;
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

std::string		Server::getRootDir()
{
	return (_rootDir);
}

std::vector<SocketListen>	*Server::getListeningSocket()
{
	return (&_listSocketListen);
}

int	Server::checkListeningSock(int sock)
{
	for (int i = 0; i < (int)_listSocketListen.size(); i++)
	{
		if (_listSocketListen[i].getSocketListen() == sock)
			return (sock);
	}
	return (-1);
}

// exception
Server::ERR_Server::ERR_Server():_error_msg("server setting failed"){}
Server::ERR_Server::ERR_Server(const char *error_msg):_error_msg(error_msg){}

const char	*Server::ERR_Server::what() const _NOEXCEPT
{
    return (_error_msg);
}
