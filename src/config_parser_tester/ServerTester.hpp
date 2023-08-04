/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ServerTester.hpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: lizhang <lizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/25 14:29:14 by lizhang       #+#    #+#                 */
/*   Updated: 2023/07/26 13:29:36 by lizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERTESTER_HPP
#define SERVERTESTER_HPP

#include "../../include/Server.hpp"
#include "DirSettingsTester.hpp"

void testServer(Server S1)
{
	
	std::cout<<"server name: "<<S1.getServerName()<<std::endl;
	std::cout<<"----------------------------------"<<std::endl;
    std::vector<int> ports = S1.getPorts();
    std::cout<<"Listening ports:"<<std::endl;
    for (unsigned int i = 0; i < ports.size(); i++);
        std::cout<<ports[i]<<std::endl;
	std::cout<<"server default settings: "<<std::endl;
	testDirSettings(S1.getRootDirSettings());
	std::cout<<"server optional directory settings: "<<std::endl;
	std::vector<DirSettings> OPTDir = S1.getOptDirSettings();
	for (unsigned int i = 0; i < OPTDir.size(); i++)
		testDirSettings(OPTDir[i]);
	std::cout<<"----server CGI directory settings: ----"<<std::endl;
	std::vector<DirSettings> CGIDir = S1.getCGIDirSettings();
	for (unsigned int i = 0; i < CGIDir.size(); i++)
		testDirSettings(CGIDir[i]);
}

#endif