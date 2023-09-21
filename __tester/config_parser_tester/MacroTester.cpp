
#include "../../include/ConfigMacros.hpp"
#include <iostream>

void getAllOfTester(void)
{
	std::string str1 = "server {\n\
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
}\n\
\n\
server {\n\
    listen 80;\n\
    server_name localhost;\n\
    root /www/server80/;\n\
    index index.html;\n\
    return 302 https://en.wikipedia.org/wiki/42_(number);\n\
}\n\
";

	std::vector<std::string> strArray =  getAllOf(str1, "server");
	if (strArray[0]!= "\n\
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
}\n")
	{
		std::cout<<"test1 fails, getAllOf is not correct!"<<std::endl;
		return ;
	}
	if (strArray[1]!= "\n\
    listen 80;\n\
    server_name localhost;\n\
    root /www/server80/;\n\
    index index.html;\n\
    return 302 https://en.wikipedia.org/wiki/42_(number);\n")
	{
		std::cout<<"test2 fails, getAllOf is not correct!"<<std::endl;
		return ;
	}
	std::cout<<"getAllOf function is correct."<<std::endl;
}

void getValueTester(void)
{
	std::string str1 = "";
	std::string result = getValue(str1, "content", 0);
	if (result != "")
	{
		std::cout<<"getValue is not correct!"<<std::endl;
		return ;
	}
	str1 = "content real content inside\
	blabla";
	result = getValue(str1, "content", 0);
	if (result != "real content inside\
	blabla")
	{
		std::cout<<"getValue is not correct!"<<std::endl;
		return ;
	}
	result = getValue(str1, "non", 0);
	if (result != "")
	{
		std::cout<<"getValue is not correct!"<<std::endl;
		return ;
	}
	str1 = "first line \n directory_list FALSE;";
	result = getValue(str1, "directory_list", 0);
	if (result != "FALSE")
	{
		std::cout<<"getValue is not correct!"<<std::endl;
		return ;
	}
	str1 = "root ./root_contents/;\n\
\n\
			index index.html;\n";
	result = getValue(str1, "index", 0);
	if (result != "index.html")
	{
		std::cout<<"getValue is not correct!"<<std::endl;
		return ;
	}
	std::cout<<"getValue function is correct."<<std::endl;
}

void charSplitTester(void)
{
	std::vector<std::string> strArray;
	std::string testArray[5] = {"This", "string", "should", "return", "multiple"};
	strArray = charSplit("This string should return multiple  ", ' ');
	for (unsigned long i = 0; i < strArray.size(); i++) {
		if (strArray[i] != testArray[i]){
			std::cout<<"charSplit function not correct!"<<std::endl;
			return ;
		}
	}
	std::cout<<"charSplit function is correct."<<std::endl;
}

void	strSplitTester(void)
{
	std::string str = "abcdefg lolol ab lol /2";
	std::vector<std::string> strArray = strSplit(str, "lol");
	std::string testArray[3] = {"abcdefg ", "lolol ab ", "lol /2"};

	for (unsigned long i = 0; i <strArray.size(); i++)
	{
		if (strArray[i] != testArray[i])
		{
			std::cout<<"strSplit function not correct!"<<std::endl;
			return ;
		}
	}
	std::cout<<"strSplit function is correct."<<std::endl;
}

void	getBracketContentTest(void)
{
	std::string str1 = "test1  {this{that{000}}} {lalala}";
	if (getBracketContent("test1", str1, 0) != "this{that{000}}")
		std::cout<<"getBracketContent failed on str1."<<std::endl;
	else
		std::cout <<"getBracketContent function is correct."<<std::endl;
}

int main(void)
{
	getAllOfTester();
	getValueTester();
	charSplitTester();
	strSplitTester();
	getBracketContentTest();
	return(0);
}