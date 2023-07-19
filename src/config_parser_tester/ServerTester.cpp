/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ServerTester.cpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: lizhang <lizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/11 18:12:11 by lizhang       #+#    #+#                 */
/*   Updated: 2023/07/19 15:42:38 by lizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"

void	testDirSettings(DirSettings D1)
{
	std::cout<<"location: "<<D1.getLocation()<<std::endl;
	if (D1.getDirType() == DEFAULT)
	{
		std::cout<<"directory type: default"<<std::endl;
	}
	if (D1.getDirType() == ROOT)
	{
		std::cout<<"directory type: root"<<std::endl;
	}
	else if (D1.getDirType() == OPTIONAL)
	{
		std::cout<<"directory type: optional"<<std::endl;
	}
	else if (D1.getDirType() == CGI)
	{
		std::cout<<"directory type: CGI"<<std::endl;
	}
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

int main(void)
{
	std::string settings = "server {\
\
listen [::]:8080 default_server;\
\
server_name www.example.com;\
\
root ./root_contents/;\
\
index index.html;\
\
allowed_methods GET, POST, DELETE;\
\
client_body_size 1000;\
\
error_page 404 /error/404/html;\
error_page 405 /error/405.html;\
\
directory_list FALSE;\
\
redirect 302 https://en.wikipedia.org/wiki/42_(number);\
\
location / {\
	allowed_methods GET;\
	directory_list FALSE;\
}\
\
location /redirect {\
	return 302 https:://eu.siteground.com/kb/domain-redirects/;\
}\
\
location /cgi-bin/*.py {\
	allowed_methods GET,POST;\
	index index.py\
	cgi.py /opt/homebrew/bin/python3;\
}\
\
\
location /cgi-bin/*.php {\
	allowed_methods GET, POST, DELETE;\
	index index.php\
	cgi .py /opt/homebrew/bin/php;\
}\
}";

	Server S1(settings);
	std::cout<<"server name: "<<S1.getServerName()<<std::endl;
	// if (S1.hasSockets() == 0)
	// 	std::cout<<"has no sockets yet."<<std::endl;
	// else
	// 	std::cout<<"has sockets."<<std::endl;
	testDirSettings(S1.getDefDirSettings());
	testDirSettings(S1.getRootDirSettings());
	std::vector<DirSettings> CGIDir = S1.getCGIDirSettings();
	testDirSettings(CGIDir[0]);
	return (0);
}