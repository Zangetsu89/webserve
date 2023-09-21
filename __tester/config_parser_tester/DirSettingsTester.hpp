/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   DirSettingsTester.hpp                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: lizhang <lizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/25 14:31:42 by lizhang       #+#    #+#                 */
/*   Updated: 2023/07/25 14:38:05 by lizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRSETTINGSTESTER_HPP
# define DIRSETTINGSTESTER_HPP

#include "../../include/DirSettings.hpp"
#include <iostream>
void	testDirSettings(DirSettings D1)
{
	std::cout<<"location: "<<D1.getLocation()<<std::endl;
	if (D1.getDirType() == DEFAULT)
	{
		std::cout<<"directory type: default"<<std::endl;
	}
	else if (D1.getDirType() == OPTIONAL)
	{
		std::cout<<"directory type: optional"<<std::endl;
	}
	else if (D1.getDirType() == CGI)
	{
		std::cout<<"directory type: CGI"<<std::endl;
	}
	std::cout<<"index page: "<<D1.getIndexPage()<<std::endl;
	std::cout<<"methods: ";
	for(unsigned int i = 0; i < D1.getMethods().size(); i++)
		std::cout<<D1.getMethods()[i]<<" ";
	std::cout<<std::endl;
	std::map<int, std::string> errorPage = D1.getErrorPage();
	std::cout<<"error page: ";
	for(auto it = errorPage.cbegin(); it!= errorPage.cend(); it++)
		std::cout<<it->first << " "<<it->second<<std::endl;
	if (D1.getDirPermission() != 0)
		std::cout<<"directory has show dirList permission."<<std::endl;
	else
		std::cout<<"directory has no show dirList permission"<<std::endl;
	std::cout<<"max body size: "<<D1.getMaxBodySize()<<std::endl;
}

#endif