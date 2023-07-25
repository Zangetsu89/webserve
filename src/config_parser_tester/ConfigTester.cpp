/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ConfigTester.cpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: lizhang <lizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/29 12:39:16 by lizhang       #+#    #+#                 */
/*   Updated: 2023/07/25 14:52:42 by lizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */


#include "../../include/Config.hpp"
#include "ServerTester.hpp"

//config file tester
int main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cout<<"please give one argument: file path"<<std::endl;
		return (0);
	}
	std::string name(av[1]);
	Config Conf(name);

	std::vector<Server>	listServers;

	listServers = Conf.getServers();
	for(unsigned int i = 0; i < listServers.size(); i++)
	{
		testServer(listServers[i]);
	}
	return(0);
}