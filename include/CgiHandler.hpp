#ifndef CGIHANDLER_HPP
# define CGIHANDLER_HPP

# include <unistd.h>
# include <fcntl.h>
# include <dirent.h>
# include <iostream>
# include <vector>

class SocketConnect;
class Request;
class Response;
class CgiHandler 
{
    private:
    	SocketConnect               *_socket;
    	Request                     *_request;
    	Response                    *_response;
    	std::string                 _cgiDir;
    	std::string                 _cgiProgram;
    	std::vector<std::string>    _cgiEnv;
    	std::vector<std::string>    _cgiArgv;

    public:
    	CgiHandler(SocketConnect *socket);
    	CgiHandler(CgiHandler const &source);
    	~CgiHandler();
    	CgiHandler &operator=(CgiHandler const &source);

    	void	makeCgiEnv();
    	void	makeCgiArgv();
    	void    responseGenerate();
    	int    prepareResponse();

    class ERR_CgiHandler : public std::exception
    {
        public:
			const char *_error_msg;
            int _error_num;
            ERR_CgiHandler();
            ERR_CgiHandler(const char *error_msg, int err);
            const char *what() const _NOEXCEPT;
    };
};


#endif