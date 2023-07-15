//
// The utility files 
//

#include "../../include/util.hpp"
#include "../../include/Server.hpp"
#include "../../include/Request.hpp"
#include "../../include/DirSettings.hpp"
#include "../../include/ConfigMacros.hpp"


// change data from vector<char> to std::string
std::string		toString(std::vector<char> *data)
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

Server	*findServer(std::string name, std::vector<Server> *list_server)
{
	std::vector<Server>::iterator	it;
	for (it = list_server->begin(); it != list_server->end(); it++)
	{
		if (name == it->getServerName())
		{
			return &(*it);
		}		
	}
	std::cout << "server(host) is not found" << std::endl;
	throw std::exception();
}

DirSettings	*findDirSetting(Request *request, Server *server) 
{
	std::string					requestLocation;
	std::vector<DirSettings> 	dir_setting;

	requestLocation = request->getRequestLocation();
	if (requestLocation.back() == '/')
		requestLocation.pop_back();

	for (; requestLocation != ""; deleteStringEnd(&requestLocation, "/"))
	{
		for (std::vector<DirSettings>::iterator it = dir_setting.begin(); it != dir_setting.end(); it++)
		{
			if (requestLocation == it->getLocation())
				return &(*it);
		}
	}
	return (server->getRootDirSettings());
}

bool	checkMethodAllowed(std::string request_method, DirSettings *setting)
{
	std::vector<std::string> allowedmethod = setting->getMethods();
	for (std::vector<std::string>::iterator it = allowedmethod.begin(); it != allowedmethod.end(); it++)
	{
		if (request_method == *it)
			return (1);
	}
	return (0);
}


