#include "../../include/Response.hpp"

Response::Response() {}

Response::Response(Request &request) {
    this->_request = request;
}

Response::Response(Response const &src) {
    *this = src;
}

Response::~Response() {}

Response &Response::operator=(Response const &rhs) {
    if (this != &rhs) {
        this->_request = rhs._request;
        this->_statusCode = rhs._statusCode;
        this->_statusMessage = rhs._statusMessage;
        this->_contentType = rhs._contentType;
        this->_contentLength = rhs._contentLength;
        this->_body = rhs._body;
    }
    return *this;
}

void Response::setRequest(Request &request) {
    this->_request = request;
}

void Response::setStatusCode(int status_code) {
    this->_statusCode = status_code;
}

void Response::setStatusMessage(std::string status_message) {
    this->_statusMessage = status_message;
}

void Response::setContentType(std::string content_type) {
    this->_contentType = content_type;
}

void Response::setContentLength(int content_length) {
    this->_contentLength = content_length;
}

void Response::setBody(std::string body) {
    this->_body = body;
}

//void Response::createResponse() {
//    std::string response = "HTTP/1.1 " + std::to_string(this->_statusCode) + " " + this->_statusMessage + "\r\n";
//    response += "Content-Type: " + this->_contentType + "\r\n";
//    response += "Content-Length: " + std::to_string(this->_contentLength) + "\r\n";
//    response += "\r\n";
//    response += this->_body;
//    this->_response = response;
//}
//
//void Response::displayResponse() {
//    std::cout << this->_response << std::endl;
//}
//
//void Response::checkMethod(Request &request, Server &server, std::string location) {
//    std::string method = request.getRequestMethod();
//    std::vector<DirSettings> dirSettings = server.getDirSettings();
//    for (std::vector<DirSettings>::iterator it = dirSettings.begin(); it != dirSettings.end(); ++it) {
//        if (it->getLocation() == location) {
//            for (std::vector<std::string>::iterator it2 = it->_requestDirSetting.getMethods().begin(); it2 != it->getMethods().end(); ++it2) {
//                if (*it2 == method) {
//                    sendResponse(method, location, request, it->getExecPath());
//                    return;
//                }
//            }
//        }
//    }
//    std::cout << "Method not allowed" << std::endl;
//}
//
//void Response::sendResponse(std::string method, std::string location, Request &request, std::string execPath) {
//    // ... (previous logic)
//
//    if (method == "GET") {
//        if (execFileExists(location, execPath) && isFile) {
//            this->_statusCode = 200;
//            this->_statusMessage = "OK";
//            this->_contentType = "text/html";
//            this->_body = request._requestBody;
//            this->_contentLength = this->_body.length();
//        } else if (locationExists(location, execPath) && isDirectory) {
//            this->_statusCode = 200;
//            this->_statusMessage = "OK";
//            this->_contentType = "text/html";
//            this->_body = generateDirectoryListing(location);
//            this->_contentLength = this->_body.length();
//        } else {
//            this->_statusCode = 404;
//            this->_statusMessage = "Not Found";
//            this->_contentType = "text/html";
//            this->_contentLength = this->_body.length();)
//        }
//    } else if (method == "POST") {
//        if (execFileExists(location, execPath) && isFile && isCGI()) {
//            // Run CGI script and generate response content
//            std::string cgiOutput = runCGIScript(request, execPath);
//            this->_statusCode = 200;
//            this->_statusMessage = "OK";
//            this->_contentType = "text/html";
//            this->_contentLength = cgiOutput.length();
//            this->_body = cgiOutput;
//        } else {
//            // handle method == delete
//
//
//        }
//    }
//
//    // Send the response
//    int ret = send(fd, this->_response.c_str(), this->_response.length(), 0);
//    if (ret == -1) {
//        std::cout << "Error: send" << std::endl;
//    }
//}
//
//std::string Response::runCGIScript(Request &request, std::string execPath) {
//    // ... (implement the logic to execute CGI script and capture its output)
//    // You can use popen or other methods to run the CGI script and capture its output
//    // Return the output as a string
//    std::cout << "CGI script output: " << std::endl;
//}
//
//std::string Response::checkLocation(Request &request, Server &server) {
//    std::string location = request.getRequestLocation();
//    std::vector<DirSettings> dirSettings = server.getDirSettings();
//    for (std::vector<DirSettings>::iterator it = dirSettings.begin(); it != dirSettings.end(); ++it) {
//        if (it->getLocation() == location) {
//            return location;
//        }
//    }
//    return "Error";
//}
//
//bool Response::execFileExists(const std::string &location, std::string execPath) {
//    std::ifstream file;
//    try {
//        file.open(execPath);
//    } catch (std::exception &e) {
//        std::cout << "Error: " << e.what() << std::endl;
//        return false;
//    }
//    std::string line;
//    while (std::getline(file, line)) {
//            this->_body += line;
//        }
//    }
//    return true;
//}
//
//std::string Response::generateDirectoryListing(const std::string &location) {
//    std::string listing = "<html><body><ul>";
//
//    for (const auto &entry : std::filesystem::directory_iterator(location)) {
//        listing += "<li><a href=\"" + entry.path().filename().string() + "\">" + entry.path().filename().string() + "</a></li>";
//    }
//
//    listing += "</ul></body></html>";
//    return listing;
//}

void Response::sendErrorResponse(SocketConnect *socketConnect) {
    std::string response = "";
    if (socketConnect->getErrorNum() == 302) {
        response = "HTTP/1.1 302 Found\r\nLocation: " + socketConnect->getRedirectURL() + "\r\n\r\n";
    }
    if (socketConnect->getErrorNum() == 400) {
        response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n"
                   "<!DOCTYPE html><html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">"
                   "<title>Error 400</title><link href=\"css.css\" rel=\"stylesheet\"></head><body>"
                   "<h1>Error 400</h1><p>Your request was not understood or is missing required parameters.</p></body></html>";

    }
    else if (socketConnect->getErrorNum() == 403) {
        response = "HTTP/1.1 403 Forbidden\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n"
                   "<!DOCTYPE html><html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">"
                   "<title>Error 403</title><link href=\"css.css\" rel=\"stylesheet\"></head><body>"
                   "<h1>Error 403</h1><p>You do not have permission to access this resource.</p></body></html>";

    }
    else if (socketConnect->getErrorNum() == 404) {
        response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n"
                   "<!DOCTYPE html><html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">"
                   "<title>Error 404</title><link href=\"css.css\" rel=\"stylesheet\"></head><body>"
                   "<h1>Error 404</h1><p>The requested page could not be found.</p></body></html>";
    }

//    std::string response = "HTTP/1.1 404 Not Found\r\n";
//    response += "Content-Type: text/html\r\n";
//    response += "Content-Length: 0\r\n";
//    response += "\r\n";
//    response += _body;
    std::cout << "SOCKETTTTTTT" << socketConnect->getNumSocket() << std::endl;
    write(socketConnect->getNumSocket(), response.c_str(), response.length());
}