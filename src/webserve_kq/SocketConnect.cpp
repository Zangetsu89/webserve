//
// Socket for Connection
//

#include "../../include/SocketConnect.hpp"
#include "../../include/util.hpp"

SocketConnect::SocketConnect(int socket, int kq):
_dataR(0), _sizeR(NOT_DEFINED), _sizeW(NOT_DEFINED), _error(-1)
{
	_numSocket = accept(socket, (struct sockaddr *) &_clientSockaddr, &_clientSockaddrLen);
	if (_numSocket < 3)
	{
		std::cout << "accept failed. _numSocket is " << _numSocket << std::endl;
		throw ERR_SocketConnect("accept new request failed");
	}
    _clientSockaddrLen = sizeof(_clientSockaddr);
	
	if (fcntl(_numSocket, F_SETFL, O_NONBLOCK) < 0)
		throw ERR_SocketConnect("fcntl failed");
	EV_SET(&_clientKevent, _numSocket, EVFILT_READ, EV_ENABLE | EV_ADD, 0, 0, this);

	if (kevent(kq, &_clientKevent, 1, NULL, 0, NULL) < 0)
		throw SocketConnect::ERR_SocketConnect("kevent for socket failed");

}

SocketConnect::~SocketConnect()
{

}

SocketConnect& SocketConnect::operator=(const SocketConnect &source)
{
	_numSocket = source._numSocket;
	_clientSockaddr = source._clientSockaddr;
	_clientSockaddrLen = source._clientSockaddrLen;
	_clientKevent = source._clientKevent;
	_dataR = source._dataR;
	_dataW = source._dataW;
	_sizeR = source._sizeR;
	_sizeW = source._sizeW;
	_clientRequest = source._clientRequest;
	_error = source._error;
	return (*this);
}

SocketConnect::SocketConnect(const SocketConnect &source)
{
	*this = source;
}

// getter

int	SocketConnect::getSocketConnect()
{
	return (_numSocket);
}

struct kevent	*SocketConnect::getKevent()
{
	return (&_clientKevent);
}

int			SocketConnect::getSizeR()
{
	return (_sizeR);
}

int			SocketConnect::getSizeW()
{
	return (_sizeW);
}

std::vector<char>	*SocketConnect::getDataR()
{
	return (&_dataR);
}

char	*SocketConnect::getDataW()
{
	char	*data_w = (char *)_dataW.c_str();
	return (data_w);
}

Request		*SocketConnect::getClientRequest()
{
	return (&_clientRequest);
}


int	SocketConnect::getError()
{
	return (_error);
}

// setter and others

void	SocketConnect::addReadData(char *buff, int size)
{
	for (int i = 0; i < size; i++)
	{
		_dataR.push_back(buff[i]);
		_sizeR++;
	}
}

void	SocketConnect::setDataW(std::string str)
{
	_dataW = str;
}

void	SocketConnect::setSizeR(int i)
{
	_sizeR = i;
}

void	SocketConnect::setSizeW(int i)
{
	_sizeW = i;
}

void	SocketConnect::printReadData()
{
	std::cout << std::endl << "[request data] - - - - - - - - - -" << std::endl << std::endl;
	for (int i = 0; i < _sizeR; i++)
	{
		write(1, &_dataR[i], 1);
	}
	std::cout << std::endl << "- - - - - - - - - - - - - - -" << std::endl << std::endl;

}

int	SocketConnect::setRequest(std::vector<char> *data_read, std::vector<Server> list_server)
{
	std::string	str_read = toString(data_read);
	std::string	oneline;

	try
	{
		oneline = splitString(&str_read, " ");
		std::cout << "method is "<< oneline << std::endl;
		_clientRequest._requestMethod = oneline;

		oneline = splitString(&str_read, " ");
		std::cout << "location is "<< oneline << std::endl;
		_clientRequest._requestLocation = oneline;

		oneline = splitString(&str_read, "\nHost: ");
		oneline = splitString(&str_read, ":");
		std::cout << "host is "<< oneline << std::endl;
		_clientRequest._requestHost = oneline;

		oneline = splitString(&str_read, "\n");
		std::cout << "port is "<< oneline << std::endl;
		_clientRequest._requestPort = oneline;
	}
	catch(const std::exception& e)
	{
		std::cout << "Request data doesn't contain correct header data" << std::endl;
		return (400);
	}
	try
	{
		_clientRequest.checkMethod(list_server);
	}
	catch(const std::exception& e)
	{
		std::cerr << "This method is not acceptable at this endpoint" << '\n';
		return (405);
	}
	
	try
	{
		oneline = splitString(&str_read, "\nContent-Length: ");
		std::cout << "content before length is "<< oneline <<  std::endl;
		_clientRequest._requestBeforeContentLength = oneline;
		oneline = splitString(&str_read, "\n");
		std::cout << "content length is "<< oneline <<  std::endl;
		_clientRequest._contentLength = stoi(oneline);
	}
	catch(const std::exception& e)
	{
		std::cout << "Request data doesn't indicate Content-Length" << std::endl;
		return (0);
	}

	try
	{
		oneline = splitString(&str_read, "\n");
		std::cout << "content body is "<< str_read <<  std::endl;
		_clientRequest._contentBody = str_read;
	}
	catch(const std::exception& e)
	{
		std::cerr << "Request data body content is not correct format" << '\n';
		return (400);
	}
	return (0);
}

void	SocketConnect::setError(int err)
{
	_error = err;
}

// exception
SocketConnect::ERR_SocketConnect::ERR_SocketConnect():_error_msg("SocketConnect setting failed"){}
SocketConnect::ERR_SocketConnect::ERR_SocketConnect(const char *error_msg):_error_msg(error_msg){}

const char	*SocketConnect::ERR_SocketConnect::what() const _NOEXCEPT
{
	return (_error_msg);
}


