/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   CgiHandler.hpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: lizhang <lizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/15 17:24:41 by lizhang       #+#    #+#                 */
/*   Updated: 2023/08/21 15:54:05 by lizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIHANDLER_HPP
#define CGIHANDLER_HPP

#include "Request.hpp"
#include "RequestHeader.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>

class CgiHandler {

    private:
        CgiHandler();
        Request _request;

    public:
    CgiHandler(Request R);
    CgiHandler(CgiHandler const &source);
    ~CgiHandler();
    CgiHandler &operator=(CgiHandler const &source);

    void    responseGenerate(char **env);
    void    prepareResponse(char **env);
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