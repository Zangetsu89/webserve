//
// Socket for Connection
//

#include "../../include/SocketConnect.hpp"

SocketConnect::SocketConnect(int socket, int kq):
_dataR(0), _dataW(0), _sizeR(NOT_DEFINED), _sizeW(NOT_DEFINED)
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
	EV_SET(&_connectKevent, _numSocket, EVFILT_READ, EV_ENABLE | EV_ADD, 0, 0, this);

	if (kevent(kq, &_connectKevent, 1, NULL, 0, NULL) < 0)
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
	_connectKevent = source._connectKevent;
	_dataR = source._dataR;
	_dataW = source._dataW;
	_sizeR = source._sizeR;
	_sizeW = source._sizeW;
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
	return (&_connectKevent);
}

void	SocketConnect::addReadData(char *buff, int size)
{
	for (int i = 0; i < size; i++)
	{
		_dataR.push_back(buff[i]);
		_sizeR++;
	}
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

std::vector<char>	*SocketConnect::getDataW()
{
	return (&_dataW);
}

// exception
SocketConnect::ERR_SocketConnect::ERR_SocketConnect():_error_msg("SocketConnect setting failed"){}
SocketConnect::ERR_SocketConnect::ERR_SocketConnect(const char *error_msg):_error_msg(error_msg){}

const char	*SocketConnect::ERR_SocketConnect::what() const _NOEXCEPT
{
	return (_error_msg);
}


