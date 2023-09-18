#ifndef WEBSERVCLI_HPP
# define WEBSERVCLI_HPP

# include <iostream>
# include <string>
# include <sys/utsname.h>
# include <exception>

class WebservCli 
{
	public:
		WebservCli(void);
		virtual ~WebservCli(void);
		WebservCli &operator=(WebservCli &source) = delete;
		WebservCli(WebservCli &source) = delete;

		void	check_os(void);
		class WrongOsException : public std::exception
		{
			private:
				const char *_error_msg;

			public:
				WrongOsException(const char *error_msg);
		    	const char *what() const _NOEXCEPT;
		};
};

#endif