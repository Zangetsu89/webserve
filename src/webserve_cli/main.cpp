#include "../../include/WebservCli.hpp"

int main(int argc, char *argv[]) {

    WebservCli WebservCli;
    if (argc < 2) {
        std::cout << "Usage: ./webserv [config_file]" << std::endl;
        return 1;
    }

    try {
        WebservCli.check_os();
    } catch (std::exception &e) {
        std::cout << "Error: " << e.what() << std::endl;
        return 1;
    }
    (void)argv;
    return (0);
}
