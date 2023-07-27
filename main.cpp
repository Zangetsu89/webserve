//
// to run the program, define one (or more) config file
// sorry it contains 6 small functions
// it will be moved into util.cpp (or something else)
//

//#include "include/WebservCli.hpp"
#include "include/Server.hpp"
#include "include/DirSettings.hpp"
#include "include/KqueueLoop.hpp"

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
		// dammy data for setting servers before connect Config function  - - - - - - - - - - - - - - - - 
		std::vector<int> ports1, ports2;
		ports1.push_back(6868);
		ports1.push_back(8080);
		ports2.push_back(6868);
	

		DirSettings ds_base1("/", 1);
		DirSettings ds1("/test/first", 1);
		DirSettings ds1_2("/test/second", 0);
		ds_base1.setMethod("GET");
		ds_base1.setIndex("index.html");
		ds_base1.setError(404, "notfound.html");
		ds_base1.setBody(1000);

		DirSettings ds_base2("/", 1);
		DirSettings ds2("/www", 0);
		ds_base2.setMethod("GET");
		ds_base2.setMethod("POST");
		ds_base2.setIndex("index.htm");
		ds_base2.setError(405, "no.html");
		ds_base2.setBody(200);
		

		std::vector<DirSettings> optDir;
		std::vector<DirSettings> optDir2;
		optDir.push_back(ds1);
		optDir.push_back(ds2);

		Server	newserver("localhost", "./www", ports1); 			// dammy (later, we use parsed info)
		Server	newserver2("www.localhost", "./test2", ports2);	// dammy (later, we use parsed info)

		newserver.setDirSettings(ds_base1, optDir);
		newserver2.setDirSettings(ds_base2, optDir2);

		newserver.setSocketListen(kq);
		newserver2.setSocketListen(kq);
		list_Servers.push_back(newserver);
		list_Servers.push_back(newserver2);

		// dammy data setting end - - - - - - - - - - - - - - - -


		KqueueLoop	mainloop(&list_Servers, kq);
		mainloop.startLoop();
	}
	catch(std::exception &e) // in this moment, any error calls exit. 
	{
		std::cout << e.what() << std::endl;
		exit(1);
	}

	return (0);
}

