//
// Socket for Connection
//

#include "include/SockConnect.hpp"
#include "include/Server.hpp"
#include "include/macro.hpp"

SockConnect::SockConnect(int socket, Server *server, int kq):
_num_Server(server), _data_r(0), _data_w(0), _size_r(NOT_DEFINED), _size_w(NOT_DEFINED)
{
	_num_socket = accept(socket, (struct sockaddr *) &_client_sockaddr, &_client_sockaddr_len);
	if (_num_socket < 3)
	{
		std::cout << "accept failed. _num_socket is " << _num_socket << std::endl;
		throw ERR_SockConnect("accept new request failed");
	}
		
    _client_sockaddr_len = sizeof(_client_sockaddr);
	
	if (fcntl(_num_socket, F_SETFL, O_NONBLOCK) < 0)
		throw ERR_SockConnect("fcntl failed");
	EV_SET(&_kev_connect, _num_socket, EVFILT_READ, EV_ENABLE | EV_ADD, 0, 0, this);

	if (kevent(kq, &_kev_connect, 1, NULL, 0, NULL) < 0)
			throw SockConnect::ERR_SockConnect("kevent for socket failed");
}

SockConnect::~SockConnect()
{

}

SockConnect& SockConnect::operator=(const SockConnect &source)
{
	_num_Server = source._num_Server;
	_num_socket = source._num_socket;
	_client_sockaddr = source._client_sockaddr;
	_client_sockaddr_len = source._client_sockaddr_len;
	_kev_connect = source._kev_connect;
	_data_r = source._data_r;
	_data_w = source._data_w;
	_size_r = source._size_r;
	_size_w = source._size_w;
	return (*this);
}

SockConnect::SockConnect(const SockConnect &source)
{
	*this = source;
}

// getter

int	SockConnect::GetSockConnect()
{
	return (_num_socket);
}

struct kevent	*SockConnect::GetKevent()
{
	return (&_kev_connect);
}

void	SockConnect::AddReadData(char *buff, int size)
{
	for (int i = 0; i < size; i++)
	{
		_data_r.push_back(buff[i]);
		_size_r++;
	}
}

void	SockConnect::PrintReadData()
{
	for (int i = 0; i < _size_r; i++)
	{
		write(1, &_data_r[i], 1);
	}
}

// exception
SockConnect::ERR_SockConnect::ERR_SockConnect():_error_msg("SockConnect setting failed"){}
SockConnect::ERR_SockConnect::ERR_SockConnect(const char *error_msg):_error_msg(error_msg){}

const char	*SockConnect::ERR_SockConnect::what() const _NOEXCEPT
{
	return (_error_msg);
}


