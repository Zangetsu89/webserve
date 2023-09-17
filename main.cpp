#include "include/WebservCli.hpp"
#include "include/Server.hpp"
#include "include/DirSettings.hpp"
#include "include/KqueueLoop.hpp"
#include "include/Config.hpp"
#include "include/macro.hpp"
#include "include/util.hpp"
#include <vector>
#include <signal.h>


void	initialCheck(int *kq, int argc)
{
	WebservCli 		WebservCli;
	WebservCli.check_os();

	if (argc < 2)
		throw "Please define config_file.";
	if ((*kq = kqueue()) < 0)
		throw "Kq failed.";
}

void	printServerInfo(bool print, std::vector<Server> list_Servers)
{
	if (!print)
		return ;
	for (size_t i = 0; i < list_Servers.size(); i++)
	{
		std::cout << i + 1 << ": server name is " << list_Servers[i].getServerName() << std::endl;
		std::cout << std::endl;
	}
}

void	continuePipe(int i)
{
	(void)i;
	std::cout << "Pipe is broken, maybe because of too much requests. just don't stop the server" << std::endl;
}

int  main(int argc, char *argv[]) 
{
	std::vector<Server>		list_Servers;
	int						kq = 0;

	signal(SIGPIPE, continuePipe);

	try
	{
		initialCheck(&kq, argc);
		Config      config(argv[1]);
		list_Servers = *config.getServers();
        if (list_Servers.size() == 0)
			throw "Error: no server information.";
		printServerInfo(TRUE, list_Servers);
		config.setKqServers(kq);

		KqueueLoop	mainloop(config.getServers(), kq);
		mainloop.startLoop();
	}
	catch(char const *text)
	{
		std::cout << "Error: " << text << std::endl;
		exit(1);
	}
	catch(std::exception &e)
	{
		std::cout << "Error: " << e.what() << std::endl;
		exit(1);
	}

	return (0);
}

