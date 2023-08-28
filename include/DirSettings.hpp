/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   DirSettings.hpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: lizhang <lizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/02 15:53:01 by lizhang       #+#    #+#                 */
/*   Updated: 2023/08/28 16:25:39 by lizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRSETTINGS_HPP
#define DIRSETTINGS_HPP

#include <string>
#include <iostream>
#include <map>
#include <limits>
#include "ConfigMacros.hpp"

enum	dirType
{
	DEFAULT,
	OPTIONAL,
	CGI
};

class	DirSettings
{
	private:
		std::string								_location;
		std::string								_index;
		int										_type;
		std::vector<std::string>				_methods;
		std::map<int, std::string>		 		_errorPage;
		bool									_dirPermission;
		std::map<int, std::string>				_redirect;
		size_t									_maxBodySize;
		std::string								_cgiDir;
		bool									checkCGI(std::string location);

	public:
		DirSettings();
		DirSettings(std::string settings);
		DirSettings(DirSettings const &another);
		~DirSettings();

		DirSettings &operator=(DirSettings const &another);

		std::string								getLocation() const;
		std::string								getIndexPage() const;
		int										getDirType() const;
		std::vector<std::string>				getMethods() const;
		std::map<int, std::string>				getErrorPage() const;
		bool									getDirPermission() const;
		std::map<int, std::string>				getRedirect() const;
		size_t 									getMaxBodySize() const;
		std::string								getCGIDir() const;

		void									setLocation(std::string root, std::string location);
		void									setIndexPage(std::string indexPage);
		void									setDirType(int dirType);
		void									setMethods(std::vector<std::string> methods);
		void									setErrorPage(std::map<int, std::string> errorPage);
		void									setDirPermission(bool permission);
		void									setRedirect(std::map<int, std::string> redirect);
		void									setMaxBodySize(size_t size);
		void									printAllDirSettings();
};

void	addDirSettingData(DirSettings &Target, DirSettings &toAdd);

#endif