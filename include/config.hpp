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
#define CONFIG_HPP

#include <string>

class Config_mac
{
    protected:
        std::string	server_name;
        std::string	listen_address;
		std::string	listen_name;
        std::string	root_location;
		std::string index;

    public:
        Config_mac(std::string file_name);
        ~Config_mac();
        std::string	get_server_name();
        std::string	get_port();
		std::string get_port_name();
        std::string	get_root();
		std::string get_index();
};

#endif