/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: lizhang <lizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/11 17:51:43 by lizhang       #+#    #+#                 */
/*   Updated: 2023/07/11 18:06:06 by lizhang       ########   odam.nl         */
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
# include "ConfigMacros.hpp"
# include "SockListen.hpp"
# include "DirSettings.hpp"
# include "ConfigMacros.hpp"


class SockListen;
class Server
{
	// private member
	private:
	std::string					_serverName;
	std::vector<SockListen>		_listenSocks;
	DirSettings					_rootDirSettings;
	std::vector<DirSettings>	_optDirSettings;
	std::vector<SockListen>		_lisSocks;

	// base member function
	protected:
	Server();	// do not use : Server must be created with port(s), name, root_dir, main kqueue

	public:
	Server(std::string server_name, std::vector<int> ports, DirSettings root, \
	std::vector<DirSettings> optional, int kq);
	~Server();
	Server& operator=(const Server &source);
	Server(const Server &source);

	// getter
	std::string					getServerName() const;
	DirSettings					getRootDirSettings() const;
	std::vector<DirSettings>	getOptDirSettings() const;
	std::vector<SockListen>		getListeningSocks() const;
	int							checkIfListeningSock(int sock); // I don't know what this should do??

	// setter
	void						addListenSocks(SockListen sock);

	// exception
	public:
	class	ERR_Server : public std::exception
	{
		private:
			const char	*_error_msg;
		public:
		ERR_Server();
		ERR_Server(const char *error_msg);
		const char *what() const _NOEXCEPT;	// _NOEXCEPT is needed since C++11
	};
};


#endif
