#include "include/WebservCli.hpp"
#include "include/Server.hpp"
#include "include/DirSettings.hpp"
#include "include/KqueueLoop.hpp"
#include "include/Config.hpp"
#include "include/macro.hpp"
#include "include/util.hpp"
#include <vector>
#include <signal.h>


void	initialCheck(int argc)
{
	WebservCli 		WebservCli;

	WebservCli.check_os();
	if (argc < 2)
		throw std::invalid_argument("Please define config_file.");
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
	std::cout << "SIGPIPE is sent, but the server must go on" << std::endl;
}

int  main(int argc, char *argv[]) 
{
	int	kq = 0;

	signal(SIGPIPE, continuePipe);
	try
	{
		initialCheck(argc);
		Config	config(argv[1]);
		printServerInfo(TRUE, *config.getServers()); // TRUE -> print server info
		if ((kq = kqueue()) < 0)
			throw std::invalid_argument("Kq failed.");
		config.setKqServers(kq);
		KqueueLoop	mainloop(config.getServers(), kq);
		mainloop.startLoop();
	}
	catch(std::exception &e)
	{
		std::cout << "Error: " << e.what() << std::endl;
		exit(1);
	}
	return (0);
}
