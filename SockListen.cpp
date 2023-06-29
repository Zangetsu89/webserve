//
// Socket for listening
//

#include "include/SockListen.hpp"
#include "include/Server.hpp"
#include "include/macro.hpp"

SockListen::SockListen(int port, Server *server, int kq): _num_Server(server)
{
	if ((_num_socket = socket(AF_INET, SOCK_STREAM, 0)) <= 0)
		throw ERR_SockListen("listening socket making failed");

	_listen_sockaddr.sin_family = AF_INET;
	_listen_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);   // INADDR_ANY is used when we don't know the IP address of our machine
	_listen_sockaddr.sin_port = htons(port); 			// The htons() function converts the unsigned short integer hostshort from host byte order to network byte order.

	if (bind(_num_socket, (struct sockaddr *) &_listen_sockaddr, sizeof(_listen_sockaddr)) < 0)
		throw ERR_SockListen("bind failed");

	if (fcntl(_num_socket, F_SETFL, O_NONBLOCK) < 0)
		throw ERR_SockListen("fcntl failed on listening socket");
	EV_SET(&_kev_listen, _num_socket, EVFILT_READ, EV_ENABLE | EV_ADD, 0, 0, NULL);

	if (kevent(kq, &_kev_listen, 1, NULL, 0, NULL) < 0)
		throw ERR_SockListen("kevent failed");

	if (listen(_num_socket, SOMAXCONN) < 0)
		throw ERR_SockListen("listen failed");
}

SockListen::~SockListen()
{

}

SockListen& SockListen::operator=(const SockListen &source)
{
	_num_Server = source._num_Server;
	_num_socket = source._num_socket;
	_listen_sockaddr = source._listen_sockaddr;
	_listen_sockaddr_len = source._listen_sockaddr_len;
	_kev_listen = source._kev_listen;
	return (*this);
}

SockListen::SockListen(const SockListen &source)
{
	*this = source;
}

// getter

int	SockListen::GetSockListen()
{
	return (_num_socket);
}

struct kevent	*SockListen::GetKevent()
{
	return (&_kev_listen);
}

// exception
SockListen::ERR_SockListen::ERR_SockListen():_error_msg("SockListen setting failed"){}
SockListen::ERR_SockListen::ERR_SockListen(const char *error_msg):_error_msg(error_msg){}

const char	*SockListen::ERR_SockListen::what() const _NOEXCEPT
{
	return (_error_msg);
}


