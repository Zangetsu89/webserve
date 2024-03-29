#include "../include/SocketListen.hpp"

SocketListen::SocketListen(int port, int kq)
{

	memset(&_listenSockaddr, 0, sizeof(_listenSockaddr));
	if ((_numSocket = socket(AF_INET, SOCK_STREAM, 0)) <= 0)
	{
		std::cout << "listening socket making failed  " << std::endl;
		throw ERR_SocketListen("listening socket making failed");
	}

	_listenSockaddr.sin_family = AF_INET;
	_listenSockaddr.sin_addr.s_addr = htonl(INADDR_ANY); // INADDR_ANY is used when we don't know the IP address of our machine
	_listenSockaddr.sin_port = htons(port);				 // The htons() function converts the unsigned short integer hostshort from host byte order to network byte order.

	int reuseport = 1;
	if ((setsockopt(_numSocket, SOL_SOCKET, SO_REUSEADDR, &reuseport, sizeof(reuseport))) < 0)
		throw ERR_SocketListen("setsockopt for reuse port failed");

	_timeout.tv_sec = 20; // set 20 second to max waiting time for data transfer
	_timeout.tv_usec = 0;
	if (setsockopt(_numSocket, SOL_SOCKET, SO_RCVTIMEO, (const char *)&_timeout, sizeof(_timeout)) < 0)
		throw ERR_SocketListen("setsockopt for timeout failed");
	
	if (bind(_numSocket, (struct sockaddr *)&_listenSockaddr, sizeof(_listenSockaddr)) < 0)
	{
		// if the same port is already opened by other server, stop.
		throw ERR_SocketListen("Can't use this socket ");
		return;
	}

	if (fcntl(_numSocket, F_SETFL, O_NONBLOCK) < 0)
		throw ERR_SocketListen("fcntl failed on listening socket");
	EV_SET(&_listenKevent, _numSocket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);

	if (kevent(kq, &_listenKevent, 1, NULL, 0, NULL) < 0)
		throw ERR_SocketListen("kevent failed");

	if (listen(_numSocket, SOMAXCONN) < 0)
		throw ERR_SocketListen("listen failed");
}

SocketListen::~SocketListen(){}

SocketListen &SocketListen::operator=(const SocketListen &source)
{
	if (this != &source)
	{
		_numSocket = source._numSocket;
		_listenSockaddr = source._listenSockaddr;
		_listenSockaddrLen = source._listenSockaddrLen;
		_listenKevent = source._listenKevent;
		_timeout = source._timeout;
	}
	return (*this);
}

SocketListen::SocketListen(const SocketListen &source)
{
	*this = source;
}

int SocketListen::getNumSocket() const
{
	return (_numSocket);
}

struct kevent *SocketListen::getKevent()
{
	return (&_listenKevent);
}

// exception
SocketListen::ERR_SocketListen::ERR_SocketListen() : _error_msg("SocketListen setting failed") {}
SocketListen::ERR_SocketListen::ERR_SocketListen(const char *error_msg) : _error_msg(error_msg) {}

const char *SocketListen::ERR_SocketListen::what() const noexcept
{
	std::cout << "Error : in SocketListen : ";
	return (_error_msg);
}
