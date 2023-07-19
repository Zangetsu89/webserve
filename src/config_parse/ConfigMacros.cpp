/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ConfigMacros.cpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: lizhang <lizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/11 14:06:52 by lizhang       #+#    #+#                 */
/*   Updated: 2023/07/19 14:29:48 by lizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ConfigMacros.hpp"

std::string getContent(std::string file_content, std::string title, size_t start_pos)
{
	size_t	pos_start;
	size_t	pos_end = start_pos;

	pos_start = file_content.find(title, start_pos);
	pos_start = pos_start + title.length();
	pos_start = file_content.find("{", pos_start);
	pos_start ++;
	pos_end = file_content.find("}", pos_end);
	return (file_content.substr(pos_start, pos_end));
}

std::string getValue(std::string content, std::string title, size_t start_pos)
{
	std::string line;
	size_t		pos = start_pos;
	size_t		pos2;

	while (pos < content.length())
	{
		pos2 = content.find(";", pos);
		line = content.substr(pos, pos2 - pos);
		if (line.find(title) < line.length())
			break ;
		pos = pos2 + 1;
	}
	pos = line.find(" ");
	return (line.substr(pos + 1, line.length() - pos - 1));
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
	start_pos = text.find("{");
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