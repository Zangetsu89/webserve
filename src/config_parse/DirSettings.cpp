/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   DirSettings.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: lizhang <lizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/03 12:24:03 by lizhang       #+#    #+#                 */
/*   Updated: 2023/07/20 17:59:57 by lizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/DirSettings.hpp"
#include <cstdlib>
#include <algorithm>
#include <iostream>

bool	DirSettings::checkCGI(std::string location)
{
	if (location.find("CGI") != (size_t)(-1) || location.find("cgi") != (size_t)(-1))
		return (1);
	return(0);
}

DirSettings::DirSettings()
{
}

DirSettings::DirSettings(DirSettings const &another)
{
	this->_location = another._location;
	this->_type = another._type;
	this->_methods = another._methods;
	this->_errorPage = another._errorPage;
	this->_dirPermission = another._dirPermission;
	this->_redirect = another._redirect;
	this->_maxBodySize = another._maxBodySize;
}

DirSettings::~DirSettings()
{
}

DirSettings &DirSettings::operator=(DirSettings const &another)
{
	this->_location = another._location;
	this->_type = another._type;
	this->_methods = another._methods;
	this->_errorPage = another._errorPage;
	this->_dirPermission = another._dirPermission;
	this->_redirect = another._redirect;
	this->_maxBodySize = another._maxBodySize;
	return (*this);
}

DirSettings::DirSettings(std::string settings)
{
	std::string					location;
	std::string					methods;
	std::vector<std::string>	errorPage;
	std::string					dirPermission;
	std::string					bodySize;
	size_t						start_pos;

	location = getValue(settings, "root", 0);
	if (location.length() > 1)
	{
		this->_type = DEFAULT;
	}
	if (location.length() < 1)
	{
		location = getValue(settings, "location", 0);
		if (location.length() == 1)
		{
			this->_type = ROOT;
		}
		else if (location.length() < 1)
			throw(std::invalid_argument("Cannot get directory location."));
		else if (this->checkCGI(location) != 0)
		{
			this->_type = CGI;
		}
		else
		{
			this->_type = OPTIONAL;
		}
	}
	this->_location = location;
	methods = getValue(settings, "allowed_methods", 0);
	this->_methods = charSplit(methods, ',');
	for (unsigned int i = 0; i < this->_methods.size(); i++)
	{
		this->_methods[i].erase(std::remove(this->_methods[i].begin(), this->_methods[i].end(), ' '), this->_methods[i].end());
	}
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
		this->_dirPermission = true;
	else
		this->_dirPermission = false;
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

int	DirSettings::getDirType() const
{
	return(this->_type);
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

std::map<int, std::string>	DirSettings::getRedirect() const
{
	return (this->_redirect);
}

size_t	DirSettings::getMaxBodySize() const
{
	return (this->_maxBodySize);
}