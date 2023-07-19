/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   DirSettings.hpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: lizhang <lizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/02 15:53:01 by lizhang       #+#    #+#                 */
/*   Updated: 2023/07/19 15:20:00 by lizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRSETTINGS_HPP
#define DIRSETTINGS_HPP

#include <string>
#include <map>
#include <limits>
#include "ConfigMacros.hpp"

enum	dirType
{
	DEFAULT,
	ROOT,
	OPTIONAL,
	CGI
};

class	DirSettings
{
	private:
		std::string								_location;
		int										_type;
		std::vector<std::string>				_methods;
		std::map<int, std::string>		 		_errorPage;
		bool									_dirPermission;
		std::map<int, std::string>				_redirect;
		size_t									_maxBodySize;
		bool									checkCGI(std::string location);

	public:
		DirSettings();
		DirSettings(std::string settings);
		DirSettings(DirSettings const &another);
		~DirSettings();

		DirSettings &operator=(DirSettings const &another);

		std::string   	  	      				getLocation() const;
		int										getDirType() const;
		std::vector<std::string>				getMethods() const;
		std::map<int, std::string>	    	  	getErrorPage() const;
		bool									getDirPermission() const;
		std::map<int, std::string>				getRedirect() const;
		size_t 									getMaxBodySize() const;
};

#endif