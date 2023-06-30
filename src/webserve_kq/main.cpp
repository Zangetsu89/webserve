//  
//   ____     ___   __  __ // ______    ____   ____ __ __ __  ____ __    __ __
//   || \\   // \\  ||\ ||    | || |    || \\ ||    || || || ||    ||    || ||
//   ||  )) ((   )) ||\\||      ||      ||_// ||==  \\ // || ||==  \\ /\ // ||
//   ||_//   \\_//  || \||      ||      || \\ ||___  \V/  || ||___  \V/\V/  ..
//  
//   only for indivisual test use (the production file is in the top directory)
//
// 6 functions for main
// main (starts from line 80)
//

#include "../../include/Server.hpp"
#include "../../include/SockConnect.hpp"
#include "../../include/SockListen.hpp"


class	ERR_exit: public std::exception
{
	public:
	const char* what() const _NOEXCEPT;
};

const char*	ERR_exit::what() const _NOEXCEPT
{
	return ("error in main part");
}

void	clear_kevent(struct kevent *kv)
{
	EV_SET(kv, -1, EVFILT_READ, EV_DISABLE, 0, 0, NULL);
}

int	check_listening_sock(int sock, std::vector<Server> *list_Servers)
{
	for (int i = 0; i < (int)list_Servers->size(); i++)
	{
		std::vector<Server> 	&temp = *list_Servers;
		Server					&tempserver = temp[i];

		int	listensock = tempserver.CheckIfListeningSock(sock);
		if (listensock > 2)
			return (listensock);
	}
	return (-1);
}

// reading function : not well done yet. (the function to make response data is needed)
void	read_sock(struct kevent *kv)
{
	SockConnect	*sock = (SockConnect *)kv->udata;
	char	buff[BUFF];
	int		r = 1;

	if (sock->GetSockConnect() < 3) // normally it never happens but if error occurs on client side, this socket number became 0... (under investigation) 
		return ;
	while (r > 0)
	{
		r = read(sock->GetSockConnect(), buff, BUFF);
		std::cout << "r is " << r << " after read from " << sock->GetSockConnect() << std::endl;
		if (r <= 0)
			break;
		sock->AddReadData(buff, r);
	}

}

// writing function : not well done yet...
void	write_sock(struct kevent *kv)
{
	SockConnect	*sock = (SockConnect *)kv->udata;

	sock->PrintReadData();					// now, it prints the read data on the terminal (later, it must send the responsdata(_data_w) to the client )
	write(sock->GetSockConnect(), "hello", 5); 	// dammy response

}


int main()
{
	std::vector<Server>		list_Servers;
	int						kq_main;
	struct kevent			kev_catch[TOTAL_KEV];
	int 					n_ev;

	// set server information
	try
	{
		for(int i = 0; i < TOTAL_KEV; i++)
			clear_kevent(&kev_catch[i]);

		kq_main = kqueue();
		if (kq_main < 0)
			throw ERR_exit();
	
		// dammy data in this moment start - - - - - - - - - - - - - - - - 
		std::vector<int> ports1, ports2;

		ports1.push_back(6868);
		ports1.push_back(8080);
		ports2.push_back(8888);

		Server	newserver(ports1, "localhost", "./", kq_main); 		// dammy (later, we use parsed info)
		Server	newserver2(ports2, "localhost", "./test2/", kq_main); // dammy (later, we use parsed info)
		list_Servers.push_back(newserver);
		list_Servers.push_back(newserver2);
		std::cout << list_Servers.size() << " server(s) set" << std::endl;

		// dammy data in this moment end - - - - - - - - - - - - - - - - 

	}
	catch(std::exception &e)
	{
		std::cout << e.what() << std::endl;
		exit(1);
	}

	// run server
	std::cout << "Kqueue start" << std::endl;
	try
	{
		while (1)
		{
			try
			{
				// catch kevent (the change of socket situation)
				n_ev = kevent(kq_main, NULL, 0, kev_catch, TOTAL_KEV, NULL);
				if (n_ev < 0)
					throw Server::ERR_Server("error on kevent");
				if (n_ev > 0)
					std::cout << std::endl << n_ev << " event(s) caught" << std::endl;
				for (int i = 0; i < n_ev; i++)
				{
					// check if the socket is closed by client (or error happens on the socket)
					if (kev_catch[i].ident == EV_ERROR)
					{
						delete((SockConnect *)kev_catch[i].udata);
						close(kev_catch[i].ident);
						throw Server::ERR_Server("error on a socket");
					}

					// check if the socket is a listening socket
					int	s_ope = check_listening_sock(kev_catch[i].ident, &list_Servers);
					if (s_ope > 2)
					{
						// make new connection socket, connect the new socket to udata of kevent
						std::cout << "an event on listenning port " << kev_catch[i].ident << std::endl;
						kev_catch[i].udata = new SockConnect((int)kev_catch[i].ident, &list_Servers[s_ope], kq_main);
						if (kev_catch[i].udata == NULL)
							throw Server::ERR_Server("error on accepting new socket");
					}
					else // the socket is a connection socket
					{
						if (kev_catch[i].filter == EVFILT_READ) // check if the socket is to read
						{
							// read and store data, make response data(not added yet), change kevent filter and set this to kev_catch
							std::cout << "EVFILT_READ on socket " << static_cast<SockConnect*>(kev_catch[i].udata)->GetSockConnect() << std::endl;
							read_sock(&kev_catch[i]);						
							EV_SET(&kev_catch[i], kev_catch[i].ident, EVFILT_READ, EV_DELETE, 0, 0, kev_catch[i].udata);
							EV_SET(&kev_catch[i], kev_catch[i].ident, EVFILT_WRITE, EV_ADD, 0, 0, kev_catch[i].udata);
							if (kevent(kq_main, &kev_catch[i], 1, NULL, 0, NULL) < 0)
								throw Server::ERR_Server("kevent for set new flag failed");
							
						}
						else if (kev_catch[i].filter == EVFILT_WRITE) // check if the socket is to write
						{
							// send response data, clean and close socket
							std::cout <<"EVFILT_WRITE on socket "  << static_cast<SockConnect*>(kev_catch[i].udata)->GetSockConnect() << std::endl;
							write_sock(&kev_catch[i]);
							std::cout << "closing " << kev_catch[i].ident << std::endl;
							delete((SockConnect *)kev_catch[i].udata);
							close(kev_catch[i].ident);
						}
					}	
				}
			}
			catch (Server::ERR_Server &e)  // we must leave the error without using exit (otherwise the server stops always)
			{
				std::cout << e.what() << std::endl;
			}
		}
	}
	catch(ERR_exit &e)
	{
		std::cout << e.what() << std::endl;
		exit(1);
	}

	return (0);
}

