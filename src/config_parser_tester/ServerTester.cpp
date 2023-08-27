/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ServerTester.cpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: lizhang <lizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/11 18:12:11 by lizhang       #+#    #+#                 */
/*   Updated: 2023/07/25 14:34:24 by lizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ServerTester.hpp"

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
location /cgi-bin-bin/*.py {\n\
	allowed_methods GET,POST;\n\
	index index.py;\n\
	cgi-bin.py /opt/homebrew/bin/python3;\n\
}\n\
\n\
\n\
location /cgi-bin-bin/*.php {\n\
	allowed_methods GET, POST, DELETE;\n\
	index index.php\n\
	cgi-bin .py /opt/homebrew/bin/php;\n\
}\n\
}";

	Server S1(settings);
	testServer(S1);
	return (0);
}