/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ResponseGen.hpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: lizhang <lizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/15 17:24:41 by lizhang       #+#    #+#                 */
/*   Updated: 2023/08/21 15:54:05 by lizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSEGEN_HPP
#define RESPONSEGEN_HPP

#include "Request.hpp"
#include "RequestHeader.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>

class Response {

    private:
        Response();
        Request _request;

    public:
    Response(Request R);
    Response(Response const &source);
    ~Response();
    Response &operator=(Response const &source);

    void    responseGenerate(char **env);
    void    prepareResponse(char **env);
    void    postRequest(Request R, char **env);

    // exception
    class ERR_Response : public std::exception
    {
        private:
            const char *_error_msg;

        public:
            int _error_num;
            ERR_Response();
            ERR_Response(const char *error_msg, int err);
            const char *what() const _NOEXCEPT; // _NOEXCEPT is needed since C++11
    };
};



#endif