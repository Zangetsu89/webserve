#include "include/WebservCli.hpp"
#include "include/Server.hpp"
#include "include/DirSettings.hpp"
#include "include/KqueueLoop.hpp"
#include "include/Config.hpp"
#include "include/macro.hpp"
#include "include/util.hpp"
#include <vector>
#include <signal.h>


int  main(int argc, char *argv[]) 
{
	WebservCli 		WebservCli;
	std::vector<Server>		list_Servers;
	int						kq = 0;

	if (argc < 2)
		returnError(1, "Please define config_file.");

	// check os and config file
	try 
	{
        WebservCli.check_os();
    }
	catch (std::exception &e) 
	{
        std::cout << "Error: " << e.what() << std::endl;
        return 1;
    }

	if ((kq = kqueue()) < 0)
		returnError(1, "Kq failed.");
	
	// set server information
	try
	{
		std::string file_path(argv[1]);
		Config      config(file_path);
		list_Servers = *config.getServers();
        if (list_Servers.size() == 0)
			exit(returnError(1, "Error: no server information."));
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
		config.setKqServers(kq);

		KqueueLoop	mainloop(config.getServers(), kq);
		mainloop.startLoop();
	}
	catch(std::exception &e) // in this moment, any error calls exit. 
	{
		std::cout << "Error: " << e.what() << std::endl;
		exit(1);
	}

	return (0);
}

