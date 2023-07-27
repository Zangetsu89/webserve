/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   DirSettings.hpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: lizhang <lizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/02 15:53:01 by lizhang       #+#    #+#                 */
/*   Updated: 2023/07/15 10:40:19 by keika         ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRSETTINGS_HPP
#define DIRSETTINGS_HPP

#include <string>
#include <map>
#include <limits>
#include "ConfigMacros.hpp"

class	DirSettings
{
	private:
		std::string								_location;
		// std::vector<std::string>				_location;
		std::vector<std::string>				_methods;
		std::vector<std::string>				_indexFile;
		std::map<int, std::string>		 		_errorPage;
		bool									_dirPermission;
		// std::map<std::string, std::string>		_dirList;
		std::map<int, std::string>				_redirect;
        size_t                                  _maxBodySize;


	public:
		DirSettings();
		DirSettings(std::string settings);
		DirSettings(std::string location, bool perm); // dammy
		~DirSettings();

		std::string						    getLocation() const;
		std::vector<std::string>			getMethods() const;
		std::vector<std::string>			getindexFile() const;
		std::map<int, std::string>	      	getErrorPage() const;
		bool								getDirPermission() const;
		std::map<std::string, std::string>  getDirList() const; //??
		std::map<int, std::string>			getRedirect() const;
		size_t 								getMaxBodySize() const;

		//dammy
		void	setMethod(std::string method);
		void	setIndex(std::string index);
		void	setError(int num, std::string error);
		void	setRedirect(int num, std::string redirect);
		void	setBody(size_t size);
};

#endif