/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   DirSettings.hpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: lizhang <lizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/02 15:53:01 by lizhang       #+#    #+#                 */
<<<<<<< HEAD
/*   Updated: 2023/07/24 16:01:39 by lizhang       ########   odam.nl         */
=======
/*   Updated: 2023/07/15 10:40:19 by keika         ########   odam.nl         */
>>>>>>> feature/kq_reconstruct
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRSETTINGS_HPP
#define DIRSETTINGS_HPP

#include <string>
#include <map>
#include <limits>
#include "ConfigMacros.hpp"

<<<<<<< HEAD
enum	dirType
{
	DEFAULT,
	OPTIONAL,
	CGI
};

=======
>>>>>>> feature/kq_reconstruct
class	DirSettings
{
	private:
		std::string								_location;
<<<<<<< HEAD
		std::string								_index;
		int										_type;
		std::vector<std::string>				_methods;
		std::map<int, std::string>		 		_errorPage;
		bool									_dirPermission;
		std::map<int, std::string>				_redirect;
		size_t									_maxBodySize;
		bool									checkCGI(std::string location);
=======
		// std::vector<std::string>				_location;
		std::vector<std::string>				_methods;
		std::vector<std::string>				_indexFile;
		std::map<int, std::string>		 		_errorPage;
		bool									_dirPermission;
		// std::map<std::string, std::string>		_dirList;
		std::map<int, std::string>				_redirect;
        size_t                                  _maxBodySize;

>>>>>>> feature/kq_reconstruct

	public:
		DirSettings();
		DirSettings(std::string settings);
<<<<<<< HEAD
		DirSettings(DirSettings const &another);
		~DirSettings();

		DirSettings &operator=(DirSettings const &another);

		std::string   	  	      				getLocation() const;
		std::string								getIndexPage() const;
		int										getDirType() const;
		std::vector<std::string>				getMethods() const;
		std::map<int, std::string>	    	  	getErrorPage() const;
		bool									getDirPermission() const;
		std::map<int, std::string>				getRedirect() const;
		size_t 									getMaxBodySize() const;

		void									setLocation(std::string root, std::string location);
		void									setIndexPage(std::string indexPage);
		void									setDirType(int dirType);
		void									setMethods(std::vector<std::string> methods);
		void									setErrorPage(std::map<int, std::string> errorPage);
		void									setDirPermission(bool permission);
		void									setRedirect(std::map<int, std::string> redirect);
		void									setMaxBodySize(size_t size);
};

void	addDirSettingData(DirSettings &Target, DirSettings &toAdd);

=======
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

>>>>>>> feature/kq_reconstruct
#endif