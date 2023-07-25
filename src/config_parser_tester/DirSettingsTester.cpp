/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   DirSettingsTester.cpp                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: lizhang <lizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/25 14:32:55 by lizhang       #+#    #+#                 */
/*   Updated: 2023/07/25 14:32:58 by lizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "DirSettingsTester.hpp"

int main(void)
{
	std::string settings = "root ./root_contents/;\n\
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
			redirect 302 https://en.wikipedia.org/wiki/42_(number)";

	std::string settings2 = "location /redirect {\n\
    redirect 302 https:://eu.siteground.com/kb/domain-redirects/;\
	}";
	DirSettings D1(settings);
	std::cout<<"test1"<<std::endl;
	testDirSettings(D1);
	DirSettings D2(settings2);
	std::cout<<"test2"<<std::endl;
	testDirSettings(D2);
	return (0);
}