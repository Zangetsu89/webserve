/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ServerTester.cpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: lizhang <lizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/11 18:12:11 by lizhang       #+#    #+#                 */
/*   Updated: 2023/07/24 16:25:45 by lizhang       ########   odam.nl         */
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

int main(void)
{
	std::string settings = "server {\n\
\n\
listen [::]:8080 default_server;\n\
\n\
server_name www.example.com;\n\
\n\
root ./root_contents/;\n\
\n\
index index.html;\n\
\n\
allowed_methods GET, POST, DELETE;\n\
\n\
client_body_size 1000;\n\
\n\
error_page 404 /error/404/html;\n\
error_page 405 /error/405.html;\n\
\n\
directory_list FALSE;\n\
\n\
redirect 302 https://en.wikipedia.org/wiki/42_(number);\n\
\n\
location / {\n\
	allowed_methods GET;\n\
	directory_list FALSE;\n\
}\n\
\n\
location /redirect {\n\
	return 302 https:://eu.siteground.com/kb/domain-redirects/;\n\
}\n\
\n\
location /cgi-bin/*.py {\n\
	allowed_methods GET,POST;\n\
	index index.py;\n\
	cgi.py /opt/homebrew/bin/python3;\n\
}\n\
\n\
\n\
location /cgi-bin/*.php {\n\
	allowed_methods GET, POST, DELETE;\n\
	index index.php\n\
	cgi .py /opt/homebrew/bin/php;\n\
}\n\
}";

	Server S1(settings);
	std::cout<<"server name: "<<S1.getServerName()<<std::endl;
	std::cout<<"server default settings: "<<std::endl;
	testDirSettings(S1.getRootDirSettings());
	std::cout<<"server optional directory settings: "<<std::endl;
	std::vector<DirSettings> OPTDir = S1.getOptDirSettings();
	testDirSettings(OPTDir[0]);
	std::cout<<"server CGI directory settings: "<<std::endl;
	std::vector<DirSettings> CGIDir = S1.getCGIDirSettings();
	testDirSettings(CGIDir[0]);
	return (0);
}