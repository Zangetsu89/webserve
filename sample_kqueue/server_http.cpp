// c++ -o serve colors.cpp server_http.cpp
// ./server

#include <sys/socket.h>
#include <sys/event.h>
#include <signal.h>
#include <netdb.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "colors.hpp"
#include <fcntl.h>
#include <vector>

#define PORT 6868
#define BUF_LEN 6000

static volatile bool running = true;

void error_exit(const char *msg)
{
	perror(msg);
	exit(1);
}

void shutdown(int)
{
	running = false;
}

typedef struct CLIENT_INFO_S
{
	std::string hostname;
	char *ipaddr;
	int port;
	char buf[BUF_LEN + 1];
	int read_size;
} CLIENT_INFO_T;

// SOMAXCON at CODAM is 128 : SOMAXCON specifies the maximum number of connection requests queued for any listening socket.
// This is maybe not an ideal way... but as sample, don't mind...
CLIENT_INFO_T client_info[SOMAXCONN];

/*==================
 * pass listening socket by argument,
 * do accept, register the new client information to client_info,
 * the return value is new socket fd for new client.
 * (if error, return -1)
 ==================*/

int accept_new_client(int sock)
{
	int new_socket;
	struct hostent *peer_host;
	struct sockaddr_in peer_sin;       // sockaddr_storage might be also possible to use
	socklen_t len = sizeof(peer_sin);

	colornote(1, 3, "START(new connection)\n", "");
	new_socket = accept(sock, (struct sockaddr *) &peer_sin, &len); //'accept' needs sockaddr as the second argument
	if (new_socket == -1)
		error_exit("accept (new connection) error");

	peer_host = gethostbyaddr((char *) &peer_sin.sin_addr.s_addr, sizeof(peer_sin.sin_addr), AF_INET);
	client_info[new_socket].hostname = (std::string) peer_host->h_name;
	client_info[new_socket].ipaddr = inet_ntoa(peer_sin.sin_addr);
	// inet_ntoa gets the ip-address from hostent struct. !! but we can't use inet_ntoa() for our webserv, actually, ip-address is not important in this moment
	client_info[new_socket].port = peer_sin.sin_port;
	std::cout << "new_socket is " << new_socket
			  << "\nsin_family | sin_port | sin_addr(ip address) | sin_zero | hostname | is \n"
			  << " | " << (int) peer_sin.sin_family << " | "
			  << client_info[new_socket].port << " | "
			  << inet_ntoa(peer_sin.sin_addr) << "|  "
			  << (int) peer_sin.sin_zero[0] << (int) peer_sin.sin_zero[1] << (int) peer_sin.sin_zero[2]
			  << (int) peer_sin.sin_zero[3]
			  << (int) peer_sin.sin_zero[4] << (int) peer_sin.sin_zero[5] << (int) peer_sin.sin_zero[6]
			  << (int) peer_sin.sin_zero[7]
			  << " | " << client_info[new_socket].hostname << " | " << std::endl;
	fcntl(new_socket, F_SETFL, O_NONBLOCK);       // needed to make write function non-blocking (error handling should be done... maybe later)
	return (new_socket);
}


/*==================
 * pass socket descriptor by argument,
 * and read the strings with read(2) by this socket,
 * (--under const--) send the string to the client
 ==================*/

void read_sock(int sock)
{
	int ret;

	client_info[sock].read_size = read(sock, client_info[sock].buf, sizeof(client_info[sock].buf) - 1);
	if (client_info[sock].read_size == -1)
		perror("read error");
	else if (client_info[sock].read_size == 0)
	{
		std::cout << "the socket " << sock << " is disconnected :" << client_info[sock].hostname << " "
				  << client_info[sock].port << std::endl;
		ret = close(sock);
		if (ret == -1)
			perror("close failed");
	}
	else
	{
		if (client_info[sock].read_size < BUF_LEN)
		{
			client_info[sock].buf[client_info[sock].read_size] = '\0';
			std::cout << client_info[sock].hostname << " fd " << sock
					  << " port " << client_info[sock].port << " has read message (" << client_info[sock].read_size
					  << ") : " << client_info[sock].buf << std::endl;
			std::cout << "Data is read! through " << sock << std::endl;
		}
		else
			std::cout << "Not enough buffer!" << sock << std::endl;
	}
}

void reply_sock(int sock)
{
	int read_size;
	int ret;

	std::cout << "!!! START WRITING !!!" << std::endl;
	write(sock, client_info[sock].buf, client_info[sock].read_size);
	write(sock, " greet from server", 19);  //  adding the comment to the received message and send back to the client
	std::cout << "wrote done " << client_info[sock].buf << std::endl << std::endl;
	ret = close(sock);
	if (ret == -1)
		perror("close failed");
}


int main(void)
{
	int sfd;
	struct sockaddr_in address;
	int sock_optval = 1;
	int ret;

	std::cout << "hello" << std::endl;
	signal(SIGINT, shutdown);

	//  Creating socket file descriptor
	if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
		error_exit("Failed to create server socket");

	//  int setsockopt(int socket, int level, int option_name, const void *option_value, socklen_t option_len);
	if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &sock_optval, sizeof(sock_optval)) == -1)
	{
		perror("setsockopt failed");
		exit(1);
	}

	// Forcefully attaching socket to the port (defined by PORT = 6868)
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);   // INADDR_ANY is used when we don't know the IP address of our machine
	address.sin_port = htons(PORT); // The htons() function converts the unsigned short integer hostshort from host byte order to network byte order.
	ret = bind(sfd, (struct sockaddr *) &address, sizeof(address));
	if (ret < 0)
		error_exit("Failed to bind server socket");

	ret = listen(sfd,SOMAXCONN);   // SOMAXCON at CODAM is 128 : SOMAXCON specifies the maximum number of connection requests queued for any listening socket.
	if (ret < 0)
		error_exit("Failed to listen to server socket");
	std::cout << "SOMAXCONN is " << SOMAXCONN << " AF_INET is " << AF_INET << std::endl;

	// set kqueue and register kevent
	int kq;
	struct kevent kev;
	kq = kqueue();
	if (kq == -1)
		error_exit("kqueue failed");

	EV_SET(&kev, sfd, EVFILT_READ, EV_ADD, 0, 0, NULL);
	ret = kevent(kq, &kev, 1, NULL, 0, NULL);   // kevent - register event
	if (ret == -1)
		error_exit("kevent to register failed");

	// start monitoring
	colornote(1, 2, "Server started...\n", "");

	while (running)
	{
		ret = kevent(kq, NULL, 0, &kev, 1, NULL);
		if (kev.ident == sfd)
		{
			int new_sock = accept_new_client(kev.ident);
			EV_SET(&kev, new_sock, EVFILT_READ, EV_ADD, 0, 0, NULL);
			ret = kevent(kq, &kev, 1, NULL, 0, NULL);
			if (ret == -1)
				error_exit("kevent for new connection failed");
		} else       // if the socket is connected-socket
		{
			int sock = kev.ident;
			if (kev.filter == EVFILT_READ)
			{
				read_sock(sock);
				EV_SET(&kev, sock, EVFILT_READ, EV_DELETE, 0, 0, NULL);
				EV_SET(&kev, sock, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
				ret = kevent(kq, &kev, 1, NULL, 0, NULL);
			} else if (kev.filter == EVFILT_WRITE)
				reply_sock(sock);

		}

	}

	close(sfd);
	colornote(1, 2, "Gracefully exiting the program", "");
	return 0;
}
