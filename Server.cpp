//
// Server
// Server class stores server information and set listening socket(s) belonging to this server
//

#include "include/Server.hpp"
#include "include/SockConnect.hpp"
#include "include/macro.hpp"


Server::Server(std::vector<int> ports, std::string name, std::string root_dir, int kq):
_servername(name), _root_dir(root_dir)
{
	if (name == "" || root_dir == "")
		throw ERR_Server("Server setting info is wrong");
	
	for (int i = 0; i < (int)ports.size(); i++)
	{
		SockListen	*tempsock = new SockListen(ports[i], this, kq);
		_listen_socks.push_back(*tempsock);
		delete(tempsock);
		std::cout << ports[i] << " port converts to socket " << _listen_socks.back().GetSockListen() << std::endl;
	}
	std::cout << _listen_socks.size() << " listening sockets are made!" << std::endl;
}

Server::~Server()
{

}

Server& Server::operator=(const Server &source)
{
	_servername = source._servername;
	_root_dir = source._root_dir;
	_listen_socks = source._listen_socks;
	return (*this);
}

Server::Server(const Server &source)
{
	*this = source;
}

// getter

std::string		Server::GetServername()
{
	return (_servername);
}

std::string		Server::GetRootDir()
{
	return (_root_dir);
}

std::vector<SockListen>	*Server::GetListeningSocks()
{
	return (&_listen_socks);
}

int	Server::CheckIfListeningSock(int sock)
{
	for (int i = 0; i < (int)_listen_socks.size(); i++)
	{
		if (_listen_socks[i].GetSockListen() == sock)
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
