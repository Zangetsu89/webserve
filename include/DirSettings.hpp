/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   DirSettings.hpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: lizhang <lizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/02 15:53:01 by lizhang       #+#    #+#                 */
/*   Updated: 2023/07/02 15:53:04 by lizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRSETTINGS_HPP
#define DIRSETTINGS_HPP

#include <string>
#include <map>
#include "ConfigMacros.hpp"

class	DirSettings
{
	private:
		std::string								_location;
		std::vector<std::string>				*_methods;
		std::map<int, std::string>		 		_errorPage;
		bool									_dirPermission;
		std::map<std::string, std::string>		_directoryList;
		std::map<int, std::string>				_redirect;

	protected:
		DirSettings();

	public:
		DirSettings(std::string settings);
		~FolderSettings();

		std::string		getLocation();
		std::string		*getMethods();
		std::string 	getErrorPage();
		std::string 	*getDirectoryList();
		std::string		getRedirect();
};

#endif