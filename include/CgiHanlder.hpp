//
// Created by Essam Dawood on 7/16/23.
//

#ifndef CGIHANLDER_HPP
#define CGIHANLDER_HPP

#include <iostream>
#include <string>
#include <unistd.h>
#include <map>
#include "RequestHandler.hpp"

class CgiHandler
{
    public:
        // Constructors
        CgiHandler(RequestHandler &req);

        // Destructor
        ~CgiHandler();

        // Setter
        void	setEnvValues(Request &request);

    private:
        Request _request;
};

#endif
