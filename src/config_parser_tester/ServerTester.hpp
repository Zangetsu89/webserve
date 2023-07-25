/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ServerTester.hpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: lizhang <lizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/25 14:29:14 by lizhang       #+#    #+#                 */
/*   Updated: 2023/07/25 14:34:35 by lizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERTESTER_HPP
#define SERVERTESTER_HPP

#include "../../include/Server.hpp"
#include "DirSettingsTester.hpp"

void testServer(Server S1)
{
	std::cout<<"server name: "<<S1.getServerName()<<std::endl;
	std::cout<<"server default settings: "<<std::endl;
	testDirSettings(S1.getRootDirSettings());
	std::cout<<"server optional directory settings: "<<std::endl;
	std::vector<DirSettings> OPTDir = S1.getOptDirSettings();
	testDirSettings(OPTDir[0]);
	std::cout<<"server CGI directory settings: "<<std::endl;
	std::vector<DirSettings> CGIDir = S1.getCGIDirSettings();
	testDirSettings(CGIDir[0]);
}

#endif