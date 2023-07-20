
#include "../../include/ConfigMacros.hpp"
#include <iostream>


void getContentTester(void)
{
	std::cout<<"No tester for getContent yet."<<std::endl;
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
	getContentTester();
	getValueTester();
	charSplitTester();
	strSplitTester();
	getBracketContentTest();
	return(0);
}