/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Config.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: lizhang <lizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/26 16:49:27 by lizhang       #+#    #+#                 */
/*   Updated: 2023/08/07 13:37:59 by keika         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <string>
# include <fstream>
# include "Server.hpp"
# include "DirSettings.hpp"
# include "ConfigMacros.hpp"

class Config
{
	private:
		std::vector<Server>		_servers;
		std::string				_readConfigFile(std::string name);

	public:
		Config(std::string file_name);
		~Config();
		// std::vector<Server>		    getServers();
        std::vector<Server>         *getServers();
		void						setKqServers(int kq);
		void						printAll();
};

#endif
