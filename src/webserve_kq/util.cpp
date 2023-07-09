//
// The utility files 
//

#include "../../include/util.hpp"


// change data from vector<char> to std::string
std::string		toString(std::vector<char> *data)
{
	std::string string(data->begin(), data->end());
	return (string);
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
	data->erase(0, pos_slice + delimita.size());
	return(newstring);
}





