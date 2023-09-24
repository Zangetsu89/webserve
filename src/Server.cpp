#include "../include/Server.hpp"
#include "../include/SocketListen.hpp"
#include "../include/DirSettings.hpp"
#include "../include/ConfigMacros.hpp"
#include "../include/macro.hpp"
#include "../include/util.hpp"

Server::Server()
{
}

Server::Server(std::string settings)
{
	std::string listen;
	std::vector<std::string> listensplit;
	std::vector<std::string> port;

	// Find the location setting parts, cut it and put into location setting list
	std::vector<std::string> location_set = strSplit_Delete(&settings, "\nlocation ");

	// Check the setting text and find 3 fundamental setting (Server name, port, root path)
	settings = removeWhitespace(settings);
	setServerName(&settings, "server_name ");
	setPorts(&settings, "listen ");
	setRootPath(&settings, "root ");

	// If other lines exists in settings -> set _rootDirSettings
	_rootDirSettings.setLocation(_rootPath, "/");
	_rootDirSettings.applyDirSettings(&settings);

	// If location setting list has some contents, set _optDirSettings
	unsigned int i;
	for (i = 0; i < location_set.size(); i++)
	{
		location_set[i] = removeWhitespace(location_set[i]);
		std::string	locate = getOneLine(location_set[i]);
		std::string	slice = splitString(&locate, " ");
		locate = removeFromRightSide(locate, "{/ ");
		if (locate == "")		// if location is "/", overwrite the rootDirSettings
			_rootDirSettings.applyDirSettings(&location_set[i]);
		else
		{
			DirSettings newDir(_rootDirSettings);
			newDir.setLocation(_rootPath, locate);
			newDir.applyDirSettings(&location_set[i]);
			_optDirSettings.push_back(newDir);
		}
	}
	if (!checkServerInfo())
		throw Exception_StopServer("Server information is not enough");
}

Server::Server(Server const &source)
{
	this->_serverName = source._serverName;
	this->_ports = source._ports;
	this->_rootPath = source._rootPath;
	this->_rootDirSettings = source._rootDirSettings;
	this->_optDirSettings = source._optDirSettings;
	// this->_cgiDirSettings = source._cgiDirSettings;
	this->_listSocketListen = source._listSocketListen;
}

Server::~Server()
{
}

Server &Server::operator=(Server const &source)
{
	this->_serverName = source._serverName;
	this->_ports = source._ports;
	this->_rootPath = source._rootPath;
	this->_rootDirSettings = source._rootDirSettings;
	this->_optDirSettings = source._optDirSettings;
	// this->_cgiDirSettings = source._cgiDirSettings;
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

std::string Server::getRootPath() const
{
	return (this->_rootPath);
}

DirSettings *Server::getRootDirSettings()
{
	return (&_rootDirSettings);
}

std::vector<DirSettings> *Server::getOptDirSettings()
{
	return (&_optDirSettings);
}

// std::vector<DirSettings> *Server::getCGIDirSettings()
// {
// 	return (&_cgiDirSettings);
// }

std::vector<SocketListen> *Server::getSocketListen()
{
	return (&_listSocketListen);
}


void	Server::setServerName(std::string *settings, std::string title)
{
	std::string str = *settings;
	std::string line;

	while (str.size())
	{
		line = splitString(&str, "\n");
		if (line.compare(0 , title.size(), title) == 0)
		{
			if (!_serverName.empty())
				throw Exception_StopServer("server_name is already set");
			_serverName = line.substr(title.size(), line.size() - title.size());
			_serverName =  removeCoronBraket(_serverName);
		}
	}
	if (_serverName.empty())
		throw Exception_StopServer("server_name not found");
}

void	Server::setRootPath(std::string *settings, std::string title)
{
	std::string str = *settings;
	std::string line;


	while (str.size())
	{
		line = splitString(&str, "\n");
		if (line.compare(0 , title.size(), title) == 0)
		{
			if (!_rootPath.empty())
				throw Exception_StopServer("rootDir is already set");
			_rootPath = line.substr(title.size(), line.size() - title.size());
			_rootPath =  removeCoronBraket(_rootPath);
		}
	}
	if (_rootPath.empty())
		throw Exception_StopServer("rootDir not found");
}

void	Server::setPorts(std::string *settings, std::string title)
{
	std::string str = *settings;
	std::string line;
	std::string listen;
	int			listen_int;

	while (str.size())
	{
		line = splitString(&str, "\n");
		if (line.compare(0 , title.size(), title) == 0)
		{
			listen = line.substr(title.size(), line.size() - title.size());
			listen = removeCoronBraket(listen);
			if (listen.compare(0, 5, "[::]:") == 0)
				listen = listen.erase(0, 5);
			listen_int = std::stoi(listen);
			if (listen_int == 0)
				throw Exception_StopServer("Port is invalid");
			_ports.push_back(listen_int);
		}
	}
	if (_ports.size() == 0)
		throw Exception_StopServer("port is not set");

}



bool	Server::checkServerInfo()
{
	if (_serverName.empty() || _ports.size() == 0 || _rootPath.empty())
		return (FALSE);
	if (_rootDirSettings.getLocation().empty())
		return (FALSE);
	if (_rootDirSettings.getIndexPage().empty())
		return (FALSE);
	if (_rootDirSettings.getMethods().size() == 0)
		return (FALSE);
	
	return (TRUE);
}
void Server::setSocketListen(int kq)
{
	for (unsigned int i = 0; i < this->_ports.size(); i++)
	{
		SocketListen tempsock(_ports[i], kq);
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
