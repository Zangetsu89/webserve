/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   DirSettings.hpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: lizhang <lizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/02 15:53:01 by lizhang       #+#    #+#                 */
/*   Updated: 2023/07/13 12:43:49 by lizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRSETTINGS_HPP
#define DIRSETTINGS_HPP

#include <string>
#include <map>
#include <limits>
#include "ConfigMacros.hpp"

#define enum
{
	DEFAULT,
	ROOT,
	OPTIONAL,
	CGI
};

class	DirSettings
{
	private:
		std::vector<std::string>				_location;
		int										_type;
		std::vector<std::string>				_methods;
		std::map<int, std::string>		 		_errorPage;
		bool									_dirPermission;
		std::map<std::string, std::string>		_dirList;
		std::map<int, std::string>				_redirect;
		size_t								_maxBodySize;
		int									setDirType();

	protected:
		DirSettings();

	public:
		DirSettings(std::string settings);
		~DirSettings();

		std::vector<std::string>           	getLocation() const;
		int									getDirType() const;
		std::vector<std::string>			getMethods() const;
		std::map<int, std::string>	      	getErrorPage() const;
		bool								getDirPermission() const;
		std::map<std::string, std::string>  getDirList() const;
		std::map<int, std::string>			getRedirect() const;
		size_t 								getMaxBodySize() const;
};

#endif