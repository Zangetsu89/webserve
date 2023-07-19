/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: lizhang <lizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/11 17:51:43 by lizhang       #+#    #+#                 */
/*   Updated: 2023/07/19 15:27:42 by lizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP
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
# include <string>
# include "ConfigMacros.hpp"
//# include "SocketListen.hpp"
# include "DirSettings.hpp"
# include "ConfigMacros.hpp"
# include "Port.hpp"


class SocketListen;
class Server
{
	// private member
	private:
	std::string						_serverName;
	std::vector<Port>				_ports;
	//std::vector<SocketListen>		_listenSockets;
	DirSettings						_defDirSettings;
	DirSettings						_rootDirSettings;
	std::vector<DirSettings>		_optDirSettings;
	std::vector<DirSettings>		_cgiDirSettings;

	// base member function
	protected:
	Server();	// do not use : Server must be created with port(s), name, root_dir, main kqueue

	public:
	Server(std::string settings);
	Server(Server const &source);
	~Server();
	Server	&operator=(Server const &source);

	// getter
	std::string					getServerName() const;
	//bool						hasSockets() const;
	DirSettings					getDefDirSettings() const;
	DirSettings					getRootDirSettings() const;
	std::vector<DirSettings>	getOptDirSettings() const;
	std::vector<DirSettings>	getCGIDirSettings() const;
	//std::vector<SocketListen>		getListeningSockets() const;
	//int						checkIfListeningSock(int sock);

	// exception
	// public:
	// class	ERR_Server : public std::exception
	// {
	// 	private:
	// 		const char	*_error_msg;
	// 	public:
	// 	ERR_Server();
	// 	ERR_Server(const char *error_msg);
	// 	const char *what() const _NOEXCEPT;	// _NOEXCEPT is needed since C++11
	// };
};


#endif
