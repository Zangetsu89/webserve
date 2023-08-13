/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   DirSettings.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: lizhang <lizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/03 12:24:03 by lizhang       #+#    #+#                 */
/*   Updated: 2023/08/04 23:57:53 by keika         ########   odam.nl         */
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
	this->_index = another._index;
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
	this->_index = another._index;
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
	std::string					index;
	std::string					methods;
	std::vector<std::string>	errorPage;
	std::string					dirPermission;
	std::string					bodySize;
	size_t						start_pos;

	this->_index = getValue(settings, "index", 0);
	location = getValue(settings, "root", 0);
	if (location.length() > 1)
	{
		this->_location = location;
		this->_type = DEFAULT;
	}
	if (location.length() < 1)
	{
		location = getValue(settings, "location", 0);
		if (location.length() == 1 && location.c_str()[0] == '/')
		{
			this->_type = DEFAULT;
		}
		else if (location.length() > 1)
		{
			setLocation(this->_location, location);
			if (this->checkCGI(location) != 0)
			{
				this->_type = CGI;
			}
			else
			{
				this->_type = OPTIONAL;
			}
		}
	}
	if (this->_location.length() < 1)
		throw(std::invalid_argument("location of directory not found"));
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

std::string	DirSettings::getIndexPage() const
{
	return(this->_index);
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

void	DirSettings::setLocation(std::string root, std::string location)
{
	if (root.size() == 0 && location.c_str()[0] != '/')
		this->_location = "/" + location;
	else if (root.size() == 0 && location.c_str()[0] == '/')
		this->_location = location;
	else if ((root.c_str()[root.length() - 1] == '/' && location.c_str()[0] != '/')||\
	(root.c_str()[root.length() - 1] != '/' && location.c_str()[0] == '/'))
		this->_location = root + location;
	else if (root.c_str()[root.length() - 1] == '/' && location.c_str()[0] == '/')
		this->_location = root + location.substr(1, location.length() - 1);
	else
		this->_location = root + "/" + location;
}

void	DirSettings::setIndexPage(std::string indexPage)
{
	this->_index = indexPage;
}

void	DirSettings::setDirType(int dirType)
{
	this->_type = dirType;
}

void	DirSettings::setMethods(std::vector<std::string> methods)
{
	this->_methods = methods;
}

void	DirSettings::setErrorPage(std::map<int, std::string> errorPage)
{
	this->_errorPage = errorPage;
}

void	DirSettings::setDirPermission(bool permission)
{
	this->_dirPermission = permission;
}


void	DirSettings::setRedirect(std::map<int, std::string> redirect)
{
	this->_redirect = redirect;
}

void	DirSettings::setMaxBodySize(size_t size)
{
	this->_maxBodySize = size;
}


void	addDirSettingData(DirSettings &Target, DirSettings &toAdd)
{
	if (toAdd.getLocation().length() > 0)
		Target.setLocation(Target.getLocation(), toAdd.getLocation());
	if (toAdd.getIndexPage().length() > 0)
		Target.setIndexPage(toAdd.getIndexPage());
	if (toAdd.getDirType() == OPTIONAL || toAdd.getDirType() == CGI)
		Target.setDirType(toAdd.getDirType());
	if (toAdd.getMethods().size() > 0)
		Target.setMethods(toAdd.getMethods());
	if (toAdd.getErrorPage().size() > 0)
		Target.setErrorPage(toAdd.getErrorPage());
	if (toAdd.getDirPermission() != Target.getDirPermission())
		Target.setDirPermission(toAdd.getDirPermission());
	if (toAdd.getRedirect().size() > 0)
		Target.setRedirect(toAdd.getRedirect());
	if (toAdd.getMaxBodySize() != std::numeric_limits<size_t>::max())
		Target.setMaxBodySize(toAdd.getMaxBodySize());
}

void	DirSettings::printAllDirSettings()
{
	std::cout << "location is " << _location << std::endl;
	std::cout << "type is " << _type << std::endl;
	for (size_t i = 0; i < _methods.size(); i++)
		std::cout << "type is " << _methods[i] << std::endl;
	std::cout << "index is " << _index << std::endl;
		
	for (auto it = _errorPage.begin(); it != _errorPage.end(); it++)
		std::cout << "errorpage is " << it->first << " " << it->second << std::endl;
	std::cout << "dir permission is " << _dirPermission << std::endl;
	std::cout << "redirect setting is " << _redirect.size() << std::endl;
	for (auto it = _redirect.begin(); it != _redirect.end(); it++)
		std::cout << "redirect is " << it->first << " " << it->second << std::endl;
	std::cout << "max body is " << _maxBodySize << std::endl;

}
