/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   SocketListen.hpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: lizhang <lizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/19 18:17:48 by lizhang       #+#    #+#                 */
/*   Updated: 2023/07/19 18:27:36 by lizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

//THIS IS A PLACEHOLDER

#ifndef SOCKETLISTEN_HPP
#define SOCKETLISTEN_HPP
# include <vector>
# include <iostream>
# include <sys/socket.h>
# include <sys/types.h>
# include <sys/event.h>
# include <sys/time.h>
# include <stdlib.h>
# include <unistd.h>
# include <netdb.h>
# include <signal.h>
# include <fcntl.h>
# include "Server.hpp"

class Server;
class SocketListen
{
	// private member
	private:
	int					_numPort;
	int					_numSocket;
	struct sockaddr_in	_listenSockaddr;
	socklen_t 			_listenSockaddrLen;
	struct kevent		_listenKevent;
	

	// base public member function
	protected:
	SocketListen();				// do not use : SocketListen must be created with socket number

	public:
	SocketListen(int port, int kq);
	~SocketListen();
	SocketListen& operator=(const SocketListen &source);
	SocketListen(const SocketListen &source);
	

	// getter and changer
	int 			getSocketListen();
	struct kevent	*getKevent();
	void			addReadData(char *buff, int size);
	void			printReadData();

	// exception
	public:
	class	ERR_SocketListen : public std::exception
	{
		private:
			const char	*_error_msg;
		public:
		ERR_SocketListen();
		ERR_SocketListen(const char *error_msg);
		const char *what() const _NOEXCEPT;	// _NOEXCEPT is needed since C++11
	};
};

#endif