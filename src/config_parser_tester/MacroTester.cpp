
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
	for (int i = 0; i < strArray.size(); i++) {
		if (strArray[i] != testArray[i]){
			std::cout<<"charSplit function not correct!"<<std::endl;
			break ;
		}
	}
	std::cout<<"charSplit function is correct."<<std::endl;
}

int main(void)
{
	getContentTester();
	getValueTester();
	charSplitTester();
	return(0);
}