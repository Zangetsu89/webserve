#include "../include/WebservCli.hpp"

WebservCli::WebservCli(void) {
}

WebservCli::~WebservCli(void) {
}

bool WebservCli::check_os(void) {
    struct utsname os;

    if (uname(&os) == -1)
        return false;
    if (std::string(os.sysname) != "Darwin")
        throw WebservCli::WrongOsException();
    return true;
}

const char *WebservCli::WrongOsException::what() const throw() 
{
    return ("OS is not mac");
}
