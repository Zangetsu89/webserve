/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   DirSettings.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: lizhang <lizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/03 12:24:03 by lizhang       #+#    #+#                 */
/*   Updated: 2023/07/03 12:24:04 by lizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/DirSettings.hpp"
#include <cstdlib>

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

	location = getValue(settings, "root", 0);
	if (location.length() < 1)
		location = getValue(settings, "location", 0);
	if (this->_location.length() < 1)
		throw(std::invalid_argument("Cannot get directory location."));
	this->_location = location;
	methods = charSplit(getValue(settings, "allowed_methods", 0), ',');
	unsigned int vecSize = methods.size();
	if (vecSize < 1)
		throw(std::invalid_argument(this->_location+" allowed methods not found."));
	for (unsigned int i = 0; i < vecSize; i++)
	{
		methods[i].erase(std::remove_if(methods[i].begin(), methods[i].end(), std::isspace), methods[i].end());
	}
	this->_methods = methods;
	//how to get multiple error page??
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
	//there can also be multiple redirects, use a for loop? and move the
	start_pos = 0;
	bodySize = getValue(settings, "client_body_size", 0);
	if (bodySize.length() > 0)
		this->_maxBodySize = atoi(bodySize.c_str());
	else
		this->_maxBodySize = std::numeric_limits<size_t>::max();
}

std::string	DirSettings::getLocation() const
{
	return(this->_location);
}

std::vector<std::string>	DirSettings::getMethods() const
{
	return(this->_methods);
}

std::map<int, std::string>	DirSettings::getErrorPage() const
{
	return (this->_errorPage);
}

bool DirSettings::getDirPermission() const
{
	return (this->_dirPermission);
}

std::map<std::string, std::string>	DirSettings::getDirList() const
{
	return (this->_dirList);
}

std::map<int, std::string>	DirSettings::getRedirect() const
{
	return (this->_redirect);
}

size_t	DirSettings::getMaxBodySize() const
{
	return (this->_maxBodySize);
}