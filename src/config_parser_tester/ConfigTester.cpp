/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tester.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: lizhang <lizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/29 12:39:16 by lizhang       #+#    #+#                 */
/*   Updated: 2023/06/29 16:16:34 by lizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */


#include "../../include/config.hpp"
#include <iostream>


//config file tester
int main(void)
{
	Config_mac Config("../../config_file/MacOS.conf");

	std::cout<<"server name: "<<Config.get_server_name()<<std::endl;
	std::cout<<"listen port: "<<Config.get_port()<<std::endl;
	std::cout<<"listen port name: "<<Config.get_port_name()<<std::endl;
	std::cout<<"root location: "<<Config.get_root()<<std::endl;
	std::cout<<"index: "<<Config.get_index()<<std::endl;
	return(0);
}