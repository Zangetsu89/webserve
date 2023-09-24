#include "../include/DirSettings.hpp"
#include "../include/util.hpp"
#include "../include/ConfigMacros.hpp"
#include "../include/macro.hpp"
#include "../include/util.hpp"
#include <limits>

DirSettings::DirSettings()
{
	_location = "/";
	_index.push_back("index.html");
	_dirPermission = FALSE;
	_maxBodySize = std::numeric_limits<size_t>::max();
}

DirSettings::DirSettings(DirSettings const &another)
{
	*this = another;
}

DirSettings::~DirSettings()
{
}

DirSettings &DirSettings::operator=(DirSettings const &another)
{
	this->_location = another._location;
	this->_index = another._index;
	this->_methods = another._methods;
	this->_errorPage = another._errorPage;
	this->_dirPermission = another._dirPermission;
	this->_redirect = another._redirect;
	this->_maxBodySize = another._maxBodySize;
	this->_cgiSetting = another._cgiSetting;
	return (*this);
}

std::string DirSettings::getLocation() const
{
	return (this->_location);
}

std::vector<std::string>	DirSettings::getIndexPage() const
{
	return (this->_index);
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

std::pair<int, std::string> DirSettings::getRedirect() const
{
	return (this->_redirect);
}

size_t DirSettings::getMaxBodySize() const
{
	return (this->_maxBodySize);
}

std::map<std::string, std::string> DirSettings::getCgiSetting() const
{
	return (this->_cgiSetting);
}

std::string 	cleanDirSettingData(std::string settings)
{
	
	std::string newstr = settings;
	newstr = removeFromBothSide(newstr, " }\n");

	return (newstr);
}

bool		checkRestOfLine(std::string line)
{
	if (line == "}")
		return (TRUE);
	if (line.compare(0, strlen("\n"), "\n") == 0)
		return (TRUE);
	if (line.compare(0, strlen("listen "), "listen ") == 0)
		return (TRUE);
	if (line.compare(0, strlen("server_name "), "server_name ") == 0)
		return (TRUE);
	if (line.compare(0, strlen("root "), "root ") == 0)
		return (TRUE);
	if (line.compare(0, strlen("location "), "location ") == 0)
		return (TRUE);
	return (FALSE);
}

void	DirSettings::applyDirSettings(std::string *settings)
{
	std::string str = *settings;
	std::string line;

	try
	{
		while (str.size())
		{
			line = splitString(&str, "\n");
			if (line == "\n" || line == "")
				continue;
			std::string index = returnValueByLine(line, "index ");
			if (index.size())
			{
				std::vector<std::string> index_list;
				index_list = charSplit(index, ',');
				_index = index_list;
				continue;
			}
			std::string methods = returnValueByLine(line, "allowed_methods ");
			if (methods.size())
			{
				std::vector<std::string> method_list;
				method_list = charSplit(methods, ',');
				for (size_t i = 0; i < method_list.size(); i++)
				{
					if (method_list[i] != "GET" && method_list[i] != "POST" && method_list[i] != "DELETE")
						throw Exception_StopServer("Invalid Method");
				}
				_methods = method_list;
				continue;
			}
			std::string error = returnValueByLine(line, "error_page ");
			if (error.size())
			{
				std::vector<std::string> error_vector;

				error_vector = charSplit(error, ' ');
				if (error_vector.size() != 2)
					throw Exception_StopServer("Error definision is wrong");
				int errnum = stoi(error_vector[0]);
				if (errnum < 400 || errnum > 600)
					throw Exception_StopServer("Error number is wrong");
				if (error_vector[1][0] != '/')
					throw Exception_StopServer("Error page setting is wrong");
				_errorPage.insert(this->_errorPage.end(), std::pair<int, std::string>(errnum, error_vector[1]));
				continue;
			}
			std::string redirect = returnValueByLine(line, "return ");
			if (redirect.size())
			{
				std::vector<std::string> redirect_vector;

				redirect_vector = charSplit(redirect, ' ');
				if (redirect_vector.size() != 2)
					throw Exception_StopServer("Error definision is wrong");
				int num = stoi(redirect_vector[0]);
				if (num < 300 || num > 400)
					throw Exception_StopServer("Redirect number is wrong");
				_redirect = std::pair<int, std::string>(num, redirect_vector[1]);
				continue;
			}
			std::string showlist = returnValueByLine(line, "directory_list ");
			if (showlist.size())
			{
				if (showlist == "TRUE")
					_dirPermission = 1;
				else if (showlist == "FALSE")
					_dirPermission = 0;
				else
					throw Exception_StopServer("Showing list setting is wrong");
				continue;
			}
			std::string bodysize = returnValueByLine(line, "client_body_size ");
			if (bodysize.size())
			{
				int bodysize_i = stoi(bodysize);
				if (bodysize_i)
					_maxBodySize = bodysize_i;
				else
					throw Exception_StopServer("max body size setting is wrong");
				continue;
			}
			std::string cgistr = returnValueByLine(line, "cgi ");
			if (cgistr.size())
			{
				std::vector<std::string> cgivector;
				cgivector = charSplit(cgistr, ' ');
				if (cgivector.size() != 2)
					throw Exception_StopServer("CGI definision is wrong");
				if (cgivector[0][0] != '.')
					throw Exception_StopServer("Extensionsetting is wrong");
				_cgiSetting.insert(this->_cgiSetting.end(), std::pair<std::string, std::string>(cgivector[0], cgivector[1]));
				continue;
			}
			if (checkRestOfLine(line) == FALSE)
				throw Exception_StopServer("Invalid line in config file");
		}
	}
	catch(const Exception_StopServer(& e))
	{
		throw Exception_StopServer(e);
	}
}

void DirSettings::setLocation(std::string root, std::string location)
{
	if (location == "/")
		this->_location = root;
	else
		this->_location = root  + location;
}

void DirSettings::setIndexPage(std::vector<std::string> indexPages)
{
	this->_index = indexPages;
}


void DirSettings::printAllDirSettings()
{
	std::cout << "location is " << _location << std::endl;
	// std::cout << "type is " << _type << std::endl;
	for (size_t i = 0; i < _methods.size(); i++)
		std::cout << "type is " << _methods[i] << std::endl;
	for (size_t i = 0; i < _index.size(); i++)
		std::cout << "index is " << _index[i] << std::endl;
	for (auto it = _errorPage.begin(); it != _errorPage.end(); it++)
		std::cout << "errorpage is " << it->first << " " << it->second << std::endl;
	std::cout << "dir permission is " <<  ((_dirPermission == 1) ? "TRUE" : "FALSE") << std::endl;
	std::cout << "redirect setting is " << _redirect.first << " " << _redirect.second << std::endl;
	std::cout << "max body is " << _maxBodySize << std::endl;
	for (auto it = _cgiSetting.begin(); it != _cgiSetting.end(); it++)
		std::cout << "Cgi is " << it->first << " " << it->second << std::endl;
}
