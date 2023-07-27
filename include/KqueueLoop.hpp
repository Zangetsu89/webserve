#ifndef KqueueLoop_HPP
#define KqueueLoop_HPP
#include <vector>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <signal.h>
#include <fcntl.h>
#include "Server.hpp"

class Server;
class KqueueLoop
{
	private:
	int				_kq_main;
	struct kevent	_kev_catch[TOTAL_KEV];
	int				_n_ev;
	std::vector<int>	_listListeningSocketInt;
	std::vector<Server>	*_servers;

	// base public member function
	protected:
	KqueueLoop(); // do not use : KqueueLoop must be created with socket number

	public:
	KqueueLoop(std::vector<Server> *servers, int kq);
	~KqueueLoop();
	KqueueLoop &operator=(const KqueueLoop &source);
	KqueueLoop(const KqueueLoop &source);


	int		checkListeningSocket(int sock);
	int 	startLoop();
	int 	getKqMain();

	// exception : after calling this exception -> exit
	public:
	class ERR_KqueueLoop : public std::exception
	{
		private:
		const char *_error_msg;

		public:
		ERR_KqueueLoop();
		ERR_KqueueLoop(const char *error_msg);
		const char *what() const _NOEXCEPT; // _NOEXCEPT is needed since C++11
	};
};

#endif
