/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ConfigMacros.cpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: lizhang <lizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/11 14:06:52 by lizhang       #+#    #+#                 */
/*   Updated: 2023/07/26 13:17:12 by lizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ConfigMacros.hpp"
#include <iostream>

std::vector<std::string> getAllOf(std::string src, std::string title)
{
	size_t start_pos = 0;
	std::vector<std::string> list;
	while (src.find(title, start_pos)!= (size_t)(-1))
	{
		start_pos = src.find(title, start_pos);
		std::string item;
		item = getBracketContent(title, src, start_pos);
		if (item.length() > 0)
			list.push_back(item);
		start_pos = start_pos + item.length();
	}
	return (list);
}


std::string getValue(std::string content, std::string title, size_t start_pos)
{
	std::string line;
	size_t		pos2;
	size_t		pos3;

	start_pos = content.find(title, start_pos);
	if (start_pos == (size_t)(-1))
		return ("");
	pos2 = content.find("\n", start_pos);
	line = content.substr(start_pos + title.length(), pos2 - start_pos - title.length());
	pos3 = line.find(";");
	if (pos3 < line.length())
		line = line.substr(0, pos3);
	while (line.c_str()[0] == ' ')
		line = line.substr(1, line.length());
	while (line.c_str()[line.length() - 1] == ' ')
		line = line.substr(0, line.length() - 1);
	while (line.c_str()[line.length() - 1] == '{')
		line = line.substr(0, line.length() - 1);
	return (line);
}

std::vector<std::string> charSplit(std::string src, char c)
{
	std::vector<std::string> strArray;
	std::string 		dup;

	dup = src;
	while (dup.length() > 0)
	{
		dup.erase(0, dup.find_first_not_of(c, 0));
		size_t found = dup.find(c, 0);
		std::string sub;
		if (found != (size_t)(-1))
			sub = dup.substr(0, found);
		else
			sub = dup;
		strArray.push_back(sub);
		dup.erase(0, sub.length());
		dup.erase(0, dup.find_first_not_of(c, 0));
	}
	return (strArray);
}

std::vector<std::string> strSplit(std::string src, std::string str)
{
	std::vector<std::string>	strArray;
	size_t						pos = 0;
	size_t						pos2 = 0;

	while (pos != (size_t)(-1))
	{
		std::string sub;
		pos2 = src.find(str, pos + str.length());
		if (pos2 != (size_t)(-1))
			sub = src.substr(pos, pos2 - pos);
		else
			sub = src.substr(pos, src.length() - pos);
		strArray.push_back(sub);
		pos = pos2;
	}
	return(strArray);
}

std::string getBracketContent (std::string name, std::string text, size_t start_pos)
{
	start_pos = text.find(name, start_pos);
	size_t	pos;
	unsigned int	bracket_count = 1;

	if (start_pos == (size_t)(-1))
		throw(std::invalid_argument("Text doesn't contain the info searched"));
	start_pos = start_pos + name.length();
	start_pos = text.find("{", start_pos);
	pos = start_pos + 1;

	std::string sub = text.substr(pos, text.length() - pos);
	char const *dup = sub.c_str();
	size_t i = 0;
	while (dup[i] && bracket_count != 0)
	{
		if (dup[i] == '{')
			bracket_count++;
		else if (dup[i] =='}')
			bracket_count--; 
		i++;
	}
	if (bracket_count != 0)
		throw(std::invalid_argument("brackets not closed"));
	return(text.substr(pos, i - 1));
}