/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   DirSettings.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: lizhang <lizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/03 12:24:03 by lizhang       #+#    #+#                 */
/*   Updated: 2023/07/14 14:29:02 by keika         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/DirSettings.hpp"
#include "../../include/util.hpp"
#include <cstdlib>
#include <algorithm>
#include <iostream>


DirSettings::DirSettings()
{
}

DirSettings::~DirSettings()
{
}

DirSettings::DirSettings(std::string settings)
{
	std::string					location;
	std::vector<std::string>	methods;
	std::vector<std::string>	errorPage;
	std::string					dirPermission;
	std::string					bodySize;
	size_t						start_pos;

	std::string	temp = getValue(settings, "root", 0);


	location = getValue(settings, "root", 0);		location = getValue(settings, "root", 0);
	if (location.length() < 1)		if (location.length() < 1)
	{		{
		location = getValue(settings, "location", 0);			location = getValue(settings, "location", 0);
	}		}
	if (location.length() < 1)		if (location.length() < 1)
		throw(std::invalid_argument("Cannot get directory location."));			throw(std::invalid_argument("Cannot get directory location."));
	this->_location = location;
	// if (temp == "/")
	// 	location.push_back("/");
	// else
	// 	location = makeStringList(&temp, "/");
	// if (location.length() < 1)
	// {
	// 	location = getValue(settings, "location", 0);
	// }
	// if (location.length() < 1)
	// 	throw(std::invalid_argument("Cannot get directory location."));
	this->_location = location;
	methods = charSplit(getValue(settings, "allowed_methods", 0), ',');
	unsigned int vecSize = methods.size();
	// if (vecSize < 1)
	// 	throw(std::invalid_argument(this->_location + " allowed methods not found."));
	for (unsigned int i = 0; i < vecSize; i++)
	{
		methods[i].erase(std::remove(methods[i].begin(), methods[i].end(), ' '), methods[i].end());
	}
	this->_methods = methods;
	std::cout << "METHOD made "  <<  std::endl;
	start_pos = 0;
	while ((start_pos = settings.find("error_page", start_pos))!= (size_t)(-1))
	{
		errorPage = charSplit(getValue(settings, "error_page", start_pos), ' ');
		if (methods.size() < 2)
			break ;
		this->_errorPage.insert(this->_errorPage.end(), std::pair<int, std::string>(atoi(errorPage[0].c_str()), errorPage[1]));
		start_pos = start_pos + 10;
	}
	dirPermission = getValue(settings, "directory_list", 0);
	if (dirPermission == "TRUE")
		this->_dirPermission = 1;
	else
		this->_dirPermission = 0;
	start_pos = 0;
	bodySize = getValue(settings, "client_body_size", 0);
	if (bodySize.length() > 0)
		this->_maxBodySize = atoi(bodySize.c_str());
	else
		this->_maxBodySize = std::numeric_limits<size_t>::max();
}

DirSettings::DirSettings(std::string location, bool perm)
{
	_location = location;
	_dirPermission = perm;
}

std::string	DirSettings::getLocation() const
{
	return(this->_location);
}

std::vector<std::string>	DirSettings::getMethods() const
{
	return(this->_methods);
}

std::vector<std::string>	DirSettings::getindexFile() const
{
	return(this->_indexFile);
}

std::map<int, std::string>	DirSettings::getErrorPage() const
{
	return (this->_errorPage);
}

bool DirSettings::getDirPermission() const
{
	return (this->_dirPermission);
}

// std::map<std::string, std::string>	DirSettings::getDirList() const
// {
// 	return (this->_dirList);
// }

std::map<int, std::string>	DirSettings::getRedirect() const
{
	return (this->_redirect);
}

size_t	DirSettings::getMaxBodySize() const
{
	return (this->_maxBodySize);
}

void	DirSettings::setMethod(std::string method)
{
	_methods.push_back(method);	
}
void	DirSettings::setIndex(std::string index)
{
 	_indexFile.push_back(index);
}
void	DirSettings::setError(int num, std::string error)
{
	std::pair<int, std::string> datapair(num, error);
	_errorPage.insert(datapair);
}

void	DirSettings::setRedirect(int num, std::string redirect)
{
	std::pair<int, std::string> datapair(num, redirect);
	_redirect.insert(datapair);
}


void	DirSettings::setBody(size_t size)
{
	_maxBodySize = size;
}