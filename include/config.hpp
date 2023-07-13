/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Config.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: lizhang <lizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/26 16:49:27 by lizhang       #+#    #+#                 */
/*   Updated: 2023/07/13 18:23:34 by lizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <string>
# include <fstream>
# include <vector>
# include "Server.hpp"
# include "DirSettings.hpp"

class Config
{
    private:
        std::vector<Server>		_servers;
		Server					_newServer(std::string settings);
		std::string				_readConfigFile(std::string name);

    public:
        Config(std::string file_name);
        ~Config();
		std::vector<Server>		    getServers() const;
        Server					    getServerAtIndex(size_t index) const;
        DirSettings                 getServerRoot();
        std::vector<DirSettings>    getServerOptional();
};

#endif

//Config takes a name, opens file, get one server at a time, cut the directory settings 
//within the server information in chuncks, use them to make Dirsettings classes and 
//then use them to make Server class. However no kq number is knowm??