#ifndef WEBSERVCLI_HPP
# define WEBSERVCLI_HPP

# include <iostream>
# include <string>
# include <sys/utsname.h>
# include <exception>

class WebservCli {
    public:
        WebservCli(void);
        virtual ~WebservCli(void);
        static bool check_os(void);
        class WrongOsException : public std::exception
        {
        public:
            virtual const char *what() const throw();
        };
};

#endif