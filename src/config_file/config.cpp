/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   config.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: lizhang <lizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/26 16:49:22 by lizhang       #+#    #+#                 */
/*   Updated: 2023/06/26 16:49:23 by lizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "config.cpp"
#include <fstream>

Config_mac::Config_mac(std::string file_name);
{
    ofstream file;
    string      lines[];

    file.open(file_name, ios::in);
    if(file.is_open() == 0)
		throw(std::invalid_argument(strerror(errno)));
	file.getline();

}

~Config_mac();
{

}

get_server_name();
get_port();
get_root();