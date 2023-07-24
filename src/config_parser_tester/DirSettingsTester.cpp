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