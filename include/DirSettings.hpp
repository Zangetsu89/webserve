#ifndef DIRSETTINGS_HPP
# define DIRSETTINGS_HPP
 
# include <string>
# include <iostream>
# include <map>
# include <vector>

class	DirSettings
{
	private:
		std::string						_location;
		std::vector<std::string>		_index;
		std::vector<std::string>		_methods;
		std::map<int, std::string>		_errorPage;
		bool							_dirPermission;
		std::pair<int, std::string>		_redirect;
		size_t							_maxBodySize;
		std::map<std::string, std::string>		_cgiSetting;
		

	public:
		DirSettings();
		DirSettings(DirSettings const &another);
		~DirSettings();
		DirSettings &operator=(DirSettings const &another);

		std::string					getLocation() const;
		std::vector<std::string>	getIndexPage() const;
		std::vector<std::string>	getMethods() const;
		std::map<int, std::string>	getErrorPage() const;
		bool						getDirPermission() const;
		std::pair<int, std::string>	getRedirect() const;
		size_t 						getMaxBodySize() const;
		std::map<std::string, std::string>	getCgiSetting() const;

		void						applyDirSettings(std::string *settings);
		void						setLocation(std::string root, std::string location);
		void						setIndexPage(std::vector<std::string> indexPages);
		void						printAllDirSettings();
};

void	addDirSettingData(DirSettings &Target, DirSettings &toAdd);

#endif