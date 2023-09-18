#include "../include/DirSettings.hpp"
#include "../include/util.hpp"
#include "../include/ConfigMacros.hpp"
#include <limits>

// bool	DirSettings::checkCGI(std::string location)
// {
// 	if (location.find("CGI") != (size_t)(-1) || location.find("cgi") != (size_t)(-1))
// 		return (1);
// 	return(0);
// }

DirSettings::DirSettings() : _cgiBool(0)
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
	this->_cgiBool = another._cgiBool;
	this->_cgiExtension = another._cgiExtension;
	this->_cgiDir = another._cgiDir;
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
	this->_cgiBool = another._cgiBool;
	this->_cgiExtension = another._cgiExtension;
	this->_cgiDir = another._cgiDir;
	return (*this);
}

DirSettings::DirSettings(std::string settings) : _cgiBool(0)
{
	std::string location;
	std::string index;
	std::string methods;
	std::vector<std::string> errorPage;
	std::string dirPermission;
	std::string bodySize;
	size_t start_pos;
	std::vector<std::string> redirect;

	this->_index = getValue(settings, "index", 0);
	location = getValue(settings, "root", 0);
	location = removeWhitespace(location);
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
			location = removeWhitespace(location);
			setLocation(this->_location, location);
			this->_type = OPTIONAL;
			setCgiElements(settings);
		}
	}
	if (this->_location.length() < 1)
		throw(std::invalid_argument("location of directory not found."));
	methods = getValue(settings, "allowed_methods ", 0);
	this->_methods = charSplit(methods, ',');
	for (unsigned int i = 0; i < this->_methods.size(); i++)
	{
		this->_methods[i].erase(std::remove(this->_methods[i].begin(), this->_methods[i].end(), ' '), this->_methods[i].end());
	}
	start_pos = 0;
	while ((start_pos = settings.find("error_page ", start_pos)) != (size_t)(-1))
	{

		errorPage = charSplit(getValue(settings, "error_page ", start_pos), ' ');
		if (errorPage.size() < 2)
			break;
		this->_errorPage.insert(this->_errorPage.end(), std::pair<int, std::string>(atoi(errorPage[0].c_str()), errorPage[1]));
		start_pos = start_pos + 11;
	}
	dirPermission = getValue(settings, "directory_list ", 0);
	if (dirPermission == "TRUE")
		this->_dirPermission = true;
	else
		this->_dirPermission = false;
	start_pos = 0;
	bodySize = getValue(settings, "client_body_size ", 0);
	if (bodySize.length() > 0)
		this->_maxBodySize = atoi(bodySize.c_str());
	else
		this->_maxBodySize = std::numeric_limits<size_t>::max();

	start_pos = 0;
	while ((start_pos = settings.find("return ", start_pos)) != (size_t)(-1))
	{

		redirect = charSplit(getValue(settings, "return ", start_pos), ' ');
		if (redirect.size() < 2)
			break;
		this->_redirect.insert(this->_redirect.end(), std::pair<int, std::string>(atoi(redirect[0].c_str()), redirect[1]));
		start_pos = start_pos + 7;
	}
	std::cout << "!! _cgiExtension, _cgiDir is " << _cgiExtension << " " << _cgiDir << std::endl;
}

std::string DirSettings::getLocation() const
{
	return (this->_location);
}

std::string DirSettings::getIndexPage() const
{
	return (this->_index);
}

int DirSettings::getDirType() const
{
	return (this->_type);
}

std::vector<std::string> DirSettings::getMethods() const
{
	return (this->_methods);
}

std::map<int, std::string> DirSettings::getErrorPage() const
{
	return (this->_errorPage);
}

bool DirSettings::getDirPermission() const
{
	return (this->_dirPermission);
}

std::map<int, std::string> DirSettings::getRedirect() const
{
	return (this->_redirect);
}

size_t DirSettings::getMaxBodySize() const
{
	return (this->_maxBodySize);
}

bool DirSettings::getCgiBool() const
{
	return (this->_cgiBool);
}

std::string DirSettings::getCgiExtension() const
{
	return (this->_cgiExtension);
}

std::string DirSettings::getCgiDir() const
{
	return (this->_cgiDir);
}

void DirSettings::setLocation(std::string root, std::string location)
{
	if (root.size() == 0 && location.c_str()[0] != '/')
		this->_location = "/" + location;
	else if (root.size() == 0 && location.c_str()[0] == '/')
		this->_location = location;
	else if ((root.c_str()[root.length() - 1] == '/' && location.c_str()[0] != '/') ||
			 (root.c_str()[root.length() - 1] != '/' && location.c_str()[0] == '/'))
		this->_location = root + location;
	else if (root.c_str()[root.length() - 1] == '/' && location.c_str()[0] == '/')
		this->_location = root + location.substr(1, location.length() - 1);
	else
		this->_location = root + "/" + location;
}

void DirSettings::setIndexPage(std::string indexPage)
{
	this->_index = indexPage;
}

void DirSettings::setDirType(int dirType)
{
	this->_type = dirType;
}

void DirSettings::setMethods(std::vector<std::string> methods)
{
	this->_methods = methods;
}

void DirSettings::setErrorPage(std::map<int, std::string> errorPage)
{
	this->_errorPage = errorPage;
}

void DirSettings::setDirPermission(bool permission)
{
	this->_dirPermission = permission;
}

void DirSettings::setRedirect(std::map<int, std::string> redirect)
{
	this->_redirect = redirect;
}

void DirSettings::setMaxBodySize(size_t size)
{
	this->_maxBodySize = size;
}

void DirSettings::setCgiBool(bool cgi)
{
	this->_cgiBool = cgi;
}

void DirSettings::setCgiExtension(std::string text)
{
	this->_cgiExtension = text;
}

void DirSettings::setCgiDir(std::string dir)
{
	this->_cgiDir = dir;
}

void DirSettings::setCgiElements(std::string settings)
{
	std::string cgi = getValue(settings, "cgi ", 0);
	std::cout << "cgi  is " << cgi << std::endl;
	if (cgi.size() == 0)
		return;
	try
	{
		std::string slice = splitString(&cgi, " ");
		splitString(&slice, ".");
		_cgiExtension = slice; // extension without dot
		_cgiDir = removeWhitespace(cgi);
		_cgiBool = 1;
		this->_type = CGI;
		std::cout << "_cgiExtension, _cgiDir is " << _cgiExtension << " " << _cgiDir << std::endl;

		if (_cgiExtension != "py" && _cgiExtension != "php")
			throw(std::invalid_argument("This server handles only .py or .php"));
	}
	catch (const std::exception &e)
	{
		std::cerr << "CGI format is not correct" << '\n';
		exit(1);
	}
}

void addDirSettingData(DirSettings &Target, DirSettings &toAdd)
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
	Target.setCgiBool(toAdd.getCgiBool());
	Target.setCgiExtension(toAdd.getCgiExtension());
	Target.setCgiDir(toAdd.getCgiDir());
}

void DirSettings::printAllDirSettings()
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
	std::cout << "_cgi is " << _cgiBool << std::endl;
	std::cout << "_cgiExtension is " << _cgiExtension << std::endl;
	std::cout << "_cgiDir is " << _cgiDir << std::endl;
}