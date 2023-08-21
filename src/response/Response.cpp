#include "../../include/Response.hpp"

Response::Response() {}

Response::Response(Request *request) {
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

Request *Response::getRequest() {
    return this->_request;
}

void Response::filterResponses(SocketConnect *socketConnect) {
    // if content-type is text/html, read the response file and store it in _body
    // Check file extention and set content-type
    // if file extention is .html, set content-type to text/html
    // if file extention is .css, set content-type to text/css
    // if file extention is .js, set content-type to text/javascript
    // if file extention is .jpg, set content-type to image/jpeg
    // if file extention is .png, set content-type to image/png
    // if file extention is .gif, set content-type to image/gif
    // if file extention is .txt, set content-type to text/plain

    this->_contentType = filterContentType(this->_request->getRequestFilePath());
    std::cout << "CONTENT TYPE: " << this->_contentType << std::endl;
    std::string filePath = this->_request->getRequestFilePath();
    readResponseFile(filePath);
    sendResponse(socketConnect);
}

void Response::readResponseFile(std::string filePath) {
    std::ifstream file;
    try {
        file.open(filePath);
        std::string line;
        while (std::getline(file, line)) {
            this->_body += line;
        }
    } catch (std::exception &e) {
        std::cout << "Error: " << e.what() << std::endl;
        return;
    }
}

std::string Response::filterContentType(std::string filePath) {
    std::string contentType = "text/plain";
    std::string extention = filePath.substr(filePath.find_last_of(".") + 1);
    if (extention == "html") {
        contentType = "text/html";
    } else if (extention == "css") {
        contentType = "text/css";
    } else if (extention == "js") {
        contentType = "text/javascript";
    } else if (extention == "jpg") {
        contentType = "image/jpeg";
    } else if (extention == "png") {
        contentType = "image/png";
    } else if (extention == "gif") {
        contentType = "image/gif";
    }
    return contentType;
}

void Response::sendResponse(SocketConnect *socketConnect) {
    std::string response = "HTTP/1.1 200 OK\r\nContent-Type: " + this->_contentType + "; charset=UTF-8\r\n\r\n" + this->_body;
//    std::string response = "HTTP/1.1 200 OK\r\n"
//                               "Content-Type: image/jpeg\r\n"
//                               "Content-Length: " + std::to_string(contentLength) + "\r\n\r\n" +
//                                this->_body;
    write(socketConnect->getNumSocket(), response.c_str(), response.length());
}

//std::streamsize Response::getImageSize(std::string filePath) {
//    std::ifstream file(filePath, std::ios::binary); // Replace "image.jpg" with your JPEG file's path
//
//    if (!file.is_open()) {
//        std::cerr << "Failed to open the file." << std::endl;
//        return 1;
//    }
//
//    // Get the current position, which is the start of the file
//    std::streampos start = file.tellg();
//
//    // Move to the end of the file
//    file.seekg(0, std::ios::end);
//
//    // Get the position at the end of the file, which is its size
//    std::streampos end = file.tellg();
//
//    // Calculate the size by subtracting start from end
//    std::streamsize fileSize = end - start;
//
//    std::cout << "File size: " << fileSize << " bytes" << std::endl;
//
//    file.close();
//    return (fileSize);
//}
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