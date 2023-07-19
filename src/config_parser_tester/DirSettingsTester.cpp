#include "../../include/DirSettings.hpp"
#include <iostream>

void	testDirSettings(DirSettings D1)
{
	std::cout<<"location: "<<D1.getLocation()<<std::endl;
	if (D1.getDirType() == DEFAULT)
	{
		std::cout<<"directory type: default"<<std::endl;
	}
	else if (D1.getDirType() == ROOT)
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
		std::cout<<"so far so good"<<std::endl;
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
	std::string settings = "root ./root_contents/;\
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
			redirect 302 https://en.wikipedia.org/wiki/42_(number)";

	std::string settings2 = "location /redirect {\
    return 302 https:://eu.siteground.com/kb/domain-redirects/;\
	}";
	DirSettings D1(settings);
	//DirSettings D2(settings2);
	std::cout<<"test1"<<std::endl;
	testDirSettings(D1);
	// std::cout<<"test2"<<std::endl;
	// testDirSettings(D2);
	return (0);
}