/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   config.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: lizhang <lizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/26 16:49:27 by lizhang       #+#    #+#                 */
/*   Updated: 2023/06/26 16:49:27 by lizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <string>
# include <fstream>
# include <vector>
# include "Server.hpp"

class Config
{
    private:
        std::vector<Server>		_servers;
		Server					newServer(std::string settings);
		std::string				readConfigFile(std::string name);

    public:
        Config(std::string file_name);
        ~Config();
		std::vector<Server>		getServers() const;
        Server					getServerAtIndex(size_t index) const;
};

#endif

//A server must have at least 3 elements: servername, root_dir and listen_socks
//A server can have more information
//maybe have a member method to store more information get additional info, with string as indicator