//
// Created by Essam Dawood on 7/16/23.
//

#include "../include/CgiHandler.hpp"

CgiHandler::CgiHandler(RequestHandler &req) : _req(&req) {
}

CgiHandler::~CgiHandler() {
}

CgiHandler::CgiHandler(const CgiHandler &src) {
    *this = src;
}

void CgiHandler::setEnvValues(const Request& request) {
    // Clear existing environment
    clearenv();

    // Set environment variables for CGI execution
    setenv("REQUEST_METHOD", request.getRequestMethod().c_str(), 1);
    setenv("REQUEST_URI", request.getRequestLocation().c_str(), 1);
    // Set other relevant environment variables as needed

    // Pass request headers through environment variables
    const std::map<std::string, std::string>& headers = request.getRequestHeaders();
    for (const auto& header : headers) {
        std::string envVarName = "HTTP_" + header.first;
        std::string envVarValue = header.second;
        setenv(envVarName.c_str(), envVarValue.c_str(), 1);
    }

    // Pass query parameters through environment variables
    const std::map<std::string, std::string>& queryParams = request.getQueryParameters();
    for (const auto& param : queryParams) {
        std::string envVarName = "QUERY_STRING_" + param.first;
        std::string envVarValue = param.second;
        setenv(envVarName.c_str(), envVarValue.c_str(), 1);
    }

    // Pass request body through environment variable
    std::string requestBody = request.getRequestBody();
    setenv("REQUEST_BODY", requestBody.c_str(), 1);

}