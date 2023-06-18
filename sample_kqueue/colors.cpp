#include <iostream>
#include "colors.hpp"

void	call_red()
{
	std::cout << "\033[31m";
}

void	call_green()
{
	std::cout << "\033[32m";
}

void	call_blue()
{
	std::cout << "\033[34m";
}

void	call_white()
{
	std::cout << "\033[37m";
}

void	reset_color()
{
	std::cout << "\033[0m";
}

void	colornote(bool printable, int color, std::string text, std::string called)
{
	if (!printable)
		return ;
	switch(color)
	{
		case 1:
			call_red();
			break ;
		case 2:
			call_green();
			break ;
		case 3:
			call_blue();
			break ;
		default:
			call_white();
	}
	std::cout << text;
	std::cout << called;
	reset_color();
}
