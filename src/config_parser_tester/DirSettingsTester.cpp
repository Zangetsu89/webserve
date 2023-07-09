#include "../../include/DirSettings.hpp"
#include <iostream>

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


	DirSettings D1(settings);
	std::cout<<"location: "<<D1.getLocation()<<std::endl;
	std::cout<<"methods: ";
	for(unsigned int i = 0; i<D1.getMethods().size(); i++)
		std::cout<<D1.getMethods()[i]<<" ";
	std::cout<<std::endl;
	std::map<int, std::string> errorPage = D1.getErrorPage();
	std::cout<<"error page: ";
	for(auto it = errorPage.cbegin(); it!= errorPage.cend(); it++)
		std::cout<<it->first << " "<<it->second<<std::endl;
	std::map<std::string, std::string> dirList = D1.getDirList();
	std::cout<<"directory list: ";
	for(auto it = dirList.cbegin(); it!= dirList.cend(); it++)
		std::cout<<it->first << " "<<it->second<<std::endl;

	return (0);
}