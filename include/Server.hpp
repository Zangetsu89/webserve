/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: lizhang <lizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/11 17:51:43 by lizhang       #+#    #+#                 */
/*   Updated: 2023/08/28 16:24:14 by lizhang       ########   odam.nl         */
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
# include "SocketListen.hpp"
# include "DirSettings.hpp"

class SocketListen;
class DirSettings;
class Server
{
	private:
	std::string						_serverName;
	std::vector<int>				_ports;
	std::string						_rootDir;
	DirSettings						_rootDirSettings;
	std::vector<DirSettings>		_optDirSettings;
	std::vector<DirSettings>		_cgiDirSettings;
	std::vector<SocketListen>		_listSocketListen;

	public:
	Server();
	Server(std::string settings);
	Server(Server const &source);
	~Server();
	Server	&operator=(Server const &source);

	// getter
	std::string					getServerName() const;
	std::vector<int>			getPorts() const;
    std::string					getRootDir() const;
	DirSettings					*getRootDirSettings();
	std::vector<DirSettings>	*getOptDirSettings();
	std::vector<DirSettings>	*getCGIDirSettings();
	std::vector<SocketListen>	*getSocketListen();

	//setter
	void						setSocketListen(int kq);
    bool                        isNumeric(std::string str);

	//exception
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
