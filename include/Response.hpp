#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <vector>
#include "Request.hpp"

class Response {

    private:
        Request _request;
        std::string _response;
        int _statusCode;
        std::string _statusMessage;
        std::string _contentType;
        int _contentLength;
        std::string _body;

    public:
        Response();
        Response(Request &request);
        Response(Response const &src);
        virtual ~Response();

        Response &operator=(Response const &rhs);

        void setRequest(Request &request);
        void setStatusCode(int status_code);
        void setStatusMessage(std::string status_message);
        void setContentType(std::string content_type);
        void setContentLength(int content_length);
        void setBody(std::string body);

        void createResponse();
        void displayResponse();
        void sendResponse(int fd);
        void checkMethod(Request &request, Server &server, std::string location);
        bool execFileExists(std::string location, std::string execPath);
        std::string checkLocation(Request &request, Server &server);

};

#endif