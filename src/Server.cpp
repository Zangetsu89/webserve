#include "../include/Server.hpp"
#include "../include/SocketListen.hpp"
#include "../include/DirSettings.hpp"
#include "../include/ConfigMacros.hpp"

Server::Server()
{
}

// CHECK later
// the _rootDir must have "/" at the end (if not, add "/" at the end)
// the _rootDir must start with "./"

Server::Server(std::string settings)
{
	std::string listen;
	std::vector<std::string> listensplit;
	std::vector<std::string> port;
	size_t pos;

	// first set up the default directory with the default content.
	// then copy the rest to the other directories if another location is found.
	try
	{
		this->_serverName = getValue(settings, "server_name ", 0);
	}
	catch (std::exception &e)
	{
		throw std::invalid_argument("No server_name in config file");
	}
	try
	{
		pos = settings.find("listen ", 0);
		listen = getValue(settings, "listen ", pos);
		if (listen.empty())
			throw std::invalid_argument("No listen in config file");
		listensplit = charSplit(listen, ' ');
		port = charSplit(listensplit[0], ':');
		if (!isNumeric(port[port.size() - 1]))
			throw std::invalid_argument("Port is not numeric");
		this->_ports.push_back(atoi(port[port.size() - 1].c_str()));
	}
	catch (std::exception &e)
	{
		throw std::invalid_argument(e.what());
	}
	pos = listen.length() + 7;
	while (settings.find("listen ", pos) != (size_t)(-1))
	{
		pos = settings.find("listen ", pos);
		listen = getValue(settings, "listen ", pos);
		listensplit = charSplit(listen, ' ');
		port = charSplit(listensplit[0], ':');
		if (!isNumeric(port[port.size() - 1]))
			throw std::invalid_argument("Port is not numeric");
		this->_ports.push_back(atoi(port[port.size() - 1].c_str()));
		pos = pos + listen.length() + 6;
	}
	std::vector<std::string> set = strSplit(settings, "location ");
	this->_rootDir = getValue(settings, "root ", 0);
	std::cout << "Root directory: " << this->_rootDir << std::endl;
	unsigned int i;
	for (i = 0; i < set.size(); i++)
	{
		DirSettings D1(set[i]);
		if (D1.getDirType() == DEFAULT)
		{
			this->_rootDirSettings = D1;
			break;
		}
	}
	set.erase(set.begin() + i);
	for (i = 0; i < set.size(); i++)
	{
		DirSettings D2(this->_rootDirSettings);
		DirSettings D3(set[i]);
		addDirSettingData(D2, D3);
		if (D2.getDirType() == OPTIONAL)
			this->_optDirSettings.push_back(D2);
		else if (D2.getDirType() == CGI)
			this->_cgiDirSettings.push_back(D2);
		else if (D2.getDirType() == DEFAULT)
			this->_rootDirSettings = D2;
	}
}

Server::Server(Server const &source)
{
	this->_serverName = source._serverName;
	this->_ports = source._ports;
	this->_rootDir = source._rootDir;
	this->_rootDirSettings = source._rootDirSettings;
	this->_optDirSettings = source._optDirSettings;
	this->_cgiDirSettings = source._cgiDirSettings;
	this->_listSocketListen = source._listSocketListen;
}

Server::~Server()
{
}

Server &Server::operator=(Server const &source)
{
	this->_serverName = source._serverName;
	this->_ports = source._ports;
	this->_rootDir = source._rootDir;
	this->_rootDirSettings = source._rootDirSettings;
	this->_optDirSettings = source._optDirSettings;
	this->_cgiDirSettings = source._cgiDirSettings;
	this->_listSocketListen = source._listSocketListen;
	return (*this);
}

std::string Server::getServerName() const
{
	return (this->_serverName);
}

std::vector<int> Server::getPorts() const
{
	return (this->_ports);
}

std::string Server::getRootDir() const
{
	return (this->_rootDir);
}

DirSettings *Server::getRootDirSettings()
{
	return (&_rootDirSettings);
}

std::vector<DirSettings> *Server::getOptDirSettings()
{
	return (&_optDirSettings);
}

std::vector<DirSettings> *Server::getCGIDirSettings()
{
	return (&_cgiDirSettings);
}

std::vector<SocketListen> *Server::getSocketListen()
{
	// std::cout << "!!getSocketListen() size is " << _listSocketListen.size() << std::endl;
	return (&_listSocketListen);
}

void Server::setSocketListen(int kq)
{
	for (unsigned int i = 0; i < this->_ports.size(); i++)
	{
		SocketListen tempsock(_ports[i], kq);
		std::cout << "setting listening port is " << _ports[i] << std::endl;
		_listSocketListen.push_back(tempsock);
	}
}

bool Server::isNumeric(std::string str)
{
	for (size_t i = 0; i < str.length(); i++)
	{
		if (!isdigit(str[i]))
			return (false);
	}
	return (true);
}
