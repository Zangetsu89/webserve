#include "include/WebservCli.hpp"
#include "include/Server.hpp"
#include "include/DirSettings.hpp"
#include "include/KqueueLoop.hpp"
#include "include/Config.hpp"

int  main(int argc, char *argv[], char **env) 
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
		list_Servers = *config.getServers();
        if (list_Servers.size() == 0)
        {
            std::cout << "Error: no server information" << std::endl;
            exit (1);
        }
		else  // print server info to check
		{
            for (size_t i = 0; i < list_Servers.size(); i++)
            {
                std::cout << std::endl;
				std::cout << "Server total size is " << list_Servers.size() << std::endl;
				std::cout << i + 1 << ": server name is " << list_Servers[i].getServerName() << std::endl;
                std::cout << std::endl;
            }
        }

		// with this function, listening sockets are set in the list servers in config class
		config.setKqServers(kq);

		KqueueLoop	mainloop(config.getServers(), kq);
		mainloop.startLoop(env);
	}
	catch(std::exception &e) // in this moment, any error calls exit. 
	{
		std::cout << e.what() << std::endl;
		exit(1);
	}

	return (0);
}

