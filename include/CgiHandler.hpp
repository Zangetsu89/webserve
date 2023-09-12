/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   CgiHandler.hpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: lizhang <lizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/15 17:24:41 by lizhang       #+#    #+#                 */
/*   Updated: 2023/09/01 13:07:47 by lizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIHANDLER_HPP
#define CGIHANDLER_HPP

// #include "SocketConnect.hpp"
// #include "Request.hpp"
// #include "Response.hpp"
#include "DirSettings.hpp"
#include "RequestHeader.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>

class SocketConnect;
class Request;
class Response;
class CgiHandler {

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
//    void    postRequest(Request R, char **env);


    // exception
    class ERR_CgiHandler : public std::exception
    {
        private:
            const char *_error_msg;

        public:
            int _error_num;
            ERR_CgiHandler();
            ERR_CgiHandler(const char *error_msg, int err);
            const char *what() const _NOEXCEPT; // _NOEXCEPT is needed since C++11
    };
};



#endif