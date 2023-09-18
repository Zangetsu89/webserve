#include "../include/WebservCli.hpp"

WebservCli::WebservCli(void) {
}

WebservCli::~WebservCli(void) {
}

void	WebservCli::check_os(void) {
	struct utsname os;

	if (uname(&os) == -1)
		throw WebservCli::WrongOsException("Invalid OS");
	if (std::string(os.sysname) != "Darwin")
		throw WebservCli::WrongOsException("OS is not mac");
}

WebservCli::WrongOsException::WrongOsException(const char *error_msg): _error_msg(error_msg)
{}

const char *WebservCli::WrongOsException::what() const _NOEXCEPT 
{
	return (_error_msg);
}
