
#include "../../include/ConfigMacros.hpp"
#include <iostream>


void getContentTester(void)
{
	std::cout<<"No tester for getContent yet."<<std::endl;
}

void getValueTester(void)
{
	std::cout<<"No tester for getValue yet."<<std::endl;
}

void charSplitTester(void)
{
	std::vector<std::string> strArray;
	std::string testArray[5] = {"This", "string", "should", "return", "multiple"};
	strArray = charSplit("This string should return multiple  ", ' ');
	for (unsigned long i = 0; i < strArray.size(); i++) {
		if (strArray[i] != testArray[i]){
			std::cout<<"charSplit function not correct!"<<std::endl;
			break ;
		}
	}
	std::cout<<"charSplit function is correct."<<std::endl;
}

void	getBracketContentTest(void)
{
	std::string str1 = "test1  {this{that{000}}} {lalala}";
	if (getBracketContent("test1", str1) != "this{that{000}}")
		std::cout<<"getBracketContent failed on str1."<<std::endl;
	else
		std::cout <<"getBracketContent function is correct."<<std::endl;
}

int main(void)
{
	getContentTester();
	getValueTester();
	charSplitTester();
	getBracketContentTest();
	return(0);
}