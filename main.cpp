//
// to run the program, define one (or more) config file
// sorry it contains 6 small functions
// it will be moved into util.cpp (or something else)
//

#include "include/WebservCli.hpp"
#include "include/Server.hpp"
#include "include/SocketConnect.hpp"
#include "include/util.hpp"


void	clearKevent(struct kevent *kv)
{
	EV_SET(kv, -1, EVFILT_READ, EV_DISABLE, 0, 0, NULL);
}

int		checkSocketListen(int sock, std::vector<Server> *list_Servers)
{
	for (int i = 0; i < (int)list_Servers->size(); i++)
	{
		std::vector<Server> 	&temp = *list_Servers;
		Server					&tempserver = temp[i];

		int	listeningsocket = tempserver.checkListeningSock(sock);
		if (listeningsocket > 2)
			return (listeningsocket);
	}
	return (-1);
}


// reading function : not well done yet. (the function to make response data is needed)
void	readSocketData(struct kevent *kv, std::vector<Server> list_server)
{
	SocketConnect	*cu_socket = (SocketConnect *)kv->udata;
	char	buff[BUFF];
	int		r = 1;

	if (cu_socket->getSocketConnect() < 3) // normally it never happens but if error occurs on client side, this socket number became 0... (under investigation) 
		return ;
	while (r > 0)
	{
		r = read(cu_socket->getSocketConnect(), buff, BUFF);
		std::cout << "r is " << r << " after read from " << cu_socket->getSocketConnect() << std::endl;
		if (r <= 0)
			break;
		cu_socket->addReadData(buff, r);
	}
	std::cout << "data size is " << cu_socket->getSizeR() << std::endl;
	cu_socket->setError(cu_socket->setRequest(cu_socket->getDataR(), list_server)); // set request class from read data and if error happens, set error number
	if (cu_socket->getError() != 0)	
		return ;

	if(cu_socket->getClientRequest()->getRequestMethod() == "GET")
	{
		// find requestLocation data and prepare return data
	}
	else if(cu_socket->getClientRequest()->getRequestMethod() == "POST")
	{
		// check CGI method and call CGI, prepare return data
	}
	else if(cu_socket->getClientRequest()->getRequestMethod() == "DELETE")
	{
		// check CGI method and call CGI to delete a file, prepare return data
	}
}

// writing function : not well done yet...
void	writeSocketData(struct kevent *kv)
{
	SocketConnect	*cu_socket = (SocketConnect *)kv->udata;

	cu_socket->printReadData();	// now, it prints the read data on the terminal (later, it must send the responsdata(_dataW) to the client )
	
	// dammy response, it returns 403 error
	cu_socket->setDataW("HTTP/1.1 403 Forbidden\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n<!DOCTYPE html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"><title>Error 403</title><link href=\"css.css\" rel=\"stylesheet\"></head><body>Error 403</body></html>");
	
	write(cu_socket->getSocketConnect(), cu_socket->getDataW() , strlen(cu_socket->getDataW())); 	// dammy response

}


int  main(int argc, char *argv[]) 
{
	WebservCli WebservCli;
	std::vector<Server>		list_Servers;
	int						kq_main;
	struct kevent			kev_catch[TOTAL_KEV];
	int 					n_ev;
	
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

	// set server information
	try
	{
		for(int i = 0; i < TOTAL_KEV; i++)
			clearKevent(&kev_catch[i]);

		kq_main = kqueue();
		if (kq_main < 0)
			throw std::exception();
	
		// dammy data in this moment start - - - - - - - - - - - - - - - - 
		std::vector<int> ports1, ports2;

		ports1.push_back(6868);
		ports1.push_back(8080);
		ports2.push_back(8080);

		Server	newserver(ports1, "localhost", "./", kq_main); 		// dammy (later, we use parsed info)
		Server	newserver2(ports2, "kito.42.fr", "./test2/", kq_main); // dammy (later, we use parsed info)
		list_Servers.push_back(newserver);
		list_Servers.push_back(newserver2);
		std::cout << list_Servers.size() << " server(s) set" << std::endl;

		// dammy data in this moment end - - - - - - - - - - - - - - - - 

	}
	catch(std::exception &e) // in this moment, any error calls exit. 
	{
		std::cout << e.what() << std::endl;
		exit(1);
	}

	// run server
	std::cout << "Kqueue start" << std::endl;

	while (1)
	{
		// catch kevent (the change of socket situation)
		n_ev = kevent(kq_main, NULL, 0, kev_catch, TOTAL_KEV, NULL);
		std::cout << std::endl << n_ev << " event(s) caught" << std::endl;

		for (int i = 0; i < n_ev; i++)
		{
			try
			{
				// check if the socket is closed by client (or error happens on the socket)
				if (kev_catch[i].ident == EV_ERROR)
				{
					delete((SocketConnect *)kev_catch[i].udata);
					close(kev_catch[i].ident);
					std::cout << "error on a socket" << std::endl;
					continue;
				}

				// check if the socket is a listening socket
				int	if_socket_listen = checkSocketListen(kev_catch[i].ident, &list_Servers);
				if (if_socket_listen > 2)
				{
					// make new connection socket, connect the new socket to udata of kevent
					std::cout << "This is an event on listenning socket " << if_socket_listen << " equal to event socket " << kev_catch[i].ident << std::endl;
					kev_catch[i].udata = new SocketConnect((int)kev_catch[i].ident, kq_main);
					if (kev_catch[i].udata == NULL)
						throw Server::ERR_Server("error on accepting new socket");
					
				}
				else // the socket is a connection socket
				{
					if (kev_catch[i].filter == EVFILT_READ) // check if the socket is to read
					{
						// read and store data, make response data(not added yet), change kevent filter and set this to kev_catch
						std::cout << "EVFILT_READ on socket " << static_cast<SocketConnect*>(kev_catch[i].udata)->getSocketConnect() << std::endl;
						readSocketData(&kev_catch[i], list_Servers);						
						EV_SET(&kev_catch[i], kev_catch[i].ident, EVFILT_READ, EV_DELETE, 0, 0, kev_catch[i].udata);
						EV_SET(&kev_catch[i], kev_catch[i].ident, EVFILT_WRITE, EV_ADD, 0, 0, kev_catch[i].udata);
						kevent(kq_main, &kev_catch[i], 1, NULL, 0, NULL);
					}
					else if (kev_catch[i].filter == EVFILT_WRITE) // check if the socket is to write
					{
						// if _error is set, send error file
						if (static_cast<SocketConnect*>(kev_catch[i].udata)->getError() != 0)
						{
							std::cout << "send error! " << static_cast<SocketConnect*>(kev_catch[i].udata)->getError() << std::endl;
						}
						else // send response data, clean and close socket
						{
							std::cout <<"EVFILT_WRITE on socket "  << static_cast<SocketConnect*>(kev_catch[i].udata)->getSocketConnect() << std::endl;
							writeSocketData(&kev_catch[i]);
						}
						std::cout << "closing " << kev_catch[i].ident << std::endl;
						delete((SocketConnect *)kev_catch[i].udata);
						close(kev_catch[i].ident);
					}
				}
			}
			catch (std::exception &e)  // we must leave the error without using exit (otherwise the server stops for one error on a stream)
			{
				std::cout << e.what() << std::endl;
			}
		}
	}

	return (0);
}

