//
// to run the program, define one (or more) config file
// sorry it contains 6 small functions
// it will be moved into util.cpp (or something else)
//

//#include "include/WebservCli.hpp"
#include "include/Server.hpp"
#include "include/DirSettings.hpp"
#include "include/KqueueLoop.hpp"
#include "include/Config.hpp"
#include "include/WebservCli.hpp"

int  main(int argc, char *argv[]) 
{
	WebservCli 				WebservCli;
	std::vector<Server>		list_Servers;
	int						kq;

	// check os and config file
	if (argc < 2)
	{
		std::cout << "Usage: ./webserv [config_file]" << std::endl;
		return 1;
	}
    try 
	{
        WebservCli.check_os();
    }
	catch (std::exception &e) 
	{
        std::cout << "Error: " << e.what() << std::endl;
        return 1;
    }
    (void)argv;

	kq = kqueue();
	if (kq < 0)
		exit(1);

	// set server information
	try
	{
		std::string file_path(argv[1]);
		Config      config(file_path);
//		list_Servers = *config.getServers();
//		std::cout<<"Listening port: "<<list_Servers[0].getPorts()[0]<<std::endl;
//		std::cout<<"Listening port: "<<list_Servers[1].getPorts()[0]<<std::endl;

		// with this function, listening sockets are set in the list servers in config class
		config.setKqServers(kq);

		// std::cout<<"Listening socket: "<<list_Servers[1].getSocketListen()[0].getSocketListen()<<std::endl;
		// KqueueLoop	mainloop(&list_Servers, kq);

		KqueueLoop	mainloop(config.getServers(), kq);	// we must use the list of servers in the Config class
		mainloop.startLoop();
	}
	catch(std::exception &e) // in this moment, any error calls exit. 
	{
		std::cout << e.what() << std::endl;
		exit(1);
	}

	return (0);
}

