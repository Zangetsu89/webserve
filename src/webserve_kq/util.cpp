//
// The utility files 
//

#include "../../include/util.hpp"


int		returnError(int err, std::string message)
{
	std::cout << message << std::endl;
	return(err);
}

std::string		vectorToString(std::vector<char> *data)
{
	std::string string(data->begin(), data->end());
	return (string);
}

std::string		removeWhitespace(std::string str)
{
	const char*	whitespace = " \t\v\r\f";

	std::string::size_type left = str.find_first_not_of(whitespace);
	if (left != std::string::npos)
	{
		std::string::size_type right = str.find_last_not_of(whitespace);
		str = str.substr(left, right - left + 1);
	}
	return (str);
}

// split the text before delimiter (and move the top of data string after delimita)
std::string		splitString(std::string *data, std::string delimita)
{
	std::string	newstring;
	size_t	pos_slice;

	pos_slice = data->find(delimita, 0);
	if (pos_slice == std::string::npos)
		throw std::exception();
	
	newstring = data->substr(0, pos_slice);
	newstring = removeWhitespace(newstring);
	data->erase(0, pos_slice + delimita.size());
	return(newstring);
}

void		deleteStringEnd(std::string *data, std::string delimita)
{
	size_t	pos_slice;

	pos_slice = data->rfind(delimita);
	if (pos_slice == std::string::npos)
	{
		std::cout << "Delimita not found " << std::endl;
		throw std::exception();
	}
	data->erase(pos_slice, data->size());
}


std::pair<std::string, std::string>	getLabelItem(std::string *line, std::string delimita)
{
	std::string	label;
	std::string	content;
	size_t	pos_slice;

	pos_slice = line->find(delimita, 0);
	if (pos_slice == std::string::npos)
	{
		throw std::exception();
	}
	label = line->substr(0, pos_slice);
	label = removeWhitespace(label);
	pos_slice += delimita.size(); 
	content = line->substr(pos_slice, line->size());
	content = removeWhitespace(content);
	std::pair<std::string, std::string>	labeldata(label, content);
	return (labeldata);
}

void	addString_toVectorChar(std::vector<char> *vect, std::string str)
{
	for (size_t i = 0; i < str.size(); i++)
		vect->push_back(str[i]);
}

std::vector<std::string>	makeStrVector(std::string param, std::string delim)
{
	std::vector<std::string>	strVector;
	size_t pos = 0;
	std::string	slicepart;
	
	while (1)
	{
		pos = param.find(delim);
		slicepart = param.substr(0, pos);
        if (slicepart.size())
		    strVector.push_back(slicepart);
		if (pos == std::string::npos)
			break;
		param = param.substr(pos + delim.size(), param.size());
		if (param.size() == 0)
			break;
	}
	return (strVector);
}


