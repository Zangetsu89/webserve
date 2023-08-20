#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <vector>
#include <filesystem>
#include "Request.hpp"
#include "SocketConnect.hpp"

class Response {

    private:
        Request         *_request;
        std::string     _response;
        int             _statusCode;
        std::string     _statusMessage;
        std::string     _contentType;
        int             _contentLength;
        std::string     _body;

    public:
        Response();
        Response(Request *request);
        Response(Response const &src);
        virtual ~Response();

        Response &operator=(Response const &rhs);

        Request *getRequest();

        void filterResponses(SocketConnect *socketConnect);
        void readResponseFile(std::string filePath);
        void sendResponse(SocketConnect *socketConnect);
//        void createResponse();
//        void displayResponse();
//        void sendResponse(int fd);
//        void checkMethod(Request &request, Server &server, std::string location);
//        bool execFileExists(std::string location, std::string execPath);
//        std::string checkLocation(Request &request, Server &server);
//        std::string generateDirectoryListing(std::string location);
        void sendErrorResponse(SocketConnect *socketConnect);

};

#endif