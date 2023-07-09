//
// Socket for listening
//

#include "../../include/SocketListen.hpp"

SocketListen::SocketListen(int port, int kq): _numPort(port)
{
	if ((_numSocket = socket(AF_INET, SOCK_STREAM, 0)) <= 0)
		throw ERR_SocketListen("listening socket making failed");

	_listenSockaddr.sin_family = AF_INET;
	_listenSockaddr.sin_addr.s_addr = htonl(INADDR_ANY);   // INADDR_ANY is used when we don't know the IP address of our machine
	_listenSockaddr.sin_port = htons(port); 			// The htons() function converts the unsigned short integer hostshort from host byte order to network byte order.

	if (bind(_numSocket, (struct sockaddr *) &_listenSockaddr, sizeof(_listenSockaddr)) < 0)
	{
		// throw ERR_SocketListen("bind failed");
		std::cout << "this listening port is already set" << std::endl;
		return ;
	}
	if (fcntl(_numSocket, F_SETFL, O_NONBLOCK) < 0)
		throw ERR_SocketListen("fcntl failed on listening socket");
	EV_SET(&_listenKevent, _numSocket, EVFILT_READ, EV_ENABLE | EV_ADD, 0, 0, NULL);

	if (kevent(kq, &_listenKevent, 1, NULL, 0, NULL) < 0)
		throw ERR_SocketListen("kevent failed");

	if (listen(_numSocket, SOMAXCONN) < 0)
		throw ERR_SocketListen("listen failed");
}

SocketListen::~SocketListen()
{

}

SocketListen& SocketListen::operator=(const SocketListen &source)
{
	_numPort = source._numPort;
	_numSocket = source._numSocket;
	_listenSockaddr = source._listenSockaddr;
	_listenSockaddrLen = source._listenSockaddrLen;
	_listenKevent = source._listenKevent;
	return (*this);
}

SocketListen::SocketListen(const SocketListen &source)
{
	*this = source;
}

// getter

int	SocketListen::getSocketListen()
{
	return (_numSocket);
}

struct kevent	*SocketListen::getKevent()
{
	return (&_listenKevent);
}

// exception
SocketListen::ERR_SocketListen::ERR_SocketListen():_error_msg("SocketListen setting failed"){}
SocketListen::ERR_SocketListen::ERR_SocketListen(const char *error_msg):_error_msg(error_msg){}

const char	*SocketListen::ERR_SocketListen::what() const _NOEXCEPT
{
	return (_error_msg);
}


