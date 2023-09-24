#ifndef KqueueLoop_HPP
# define KqueueLoop_HPP
# include <iostream>
# include <vector>
# include <sys/socket.h>
# include <sys/event.h>
# include <fcntl.h>
# include "Server.hpp"
# include "macro.hpp"

class Server;
class KqueueLoop
{
	private:
		int					_kq_main;
		struct kevent		_kev_catch[TOTAL_KEV];
		int					_n_ev;
		std::vector<Server>	*_servers;

	protected:
		KqueueLoop();

	public:
		KqueueLoop(std::vector<Server> *servers, int kq);
		~KqueueLoop();
		KqueueLoop &operator=(const KqueueLoop &source) = delete;	// copy prohibited
		KqueueLoop(const KqueueLoop &source) = delete;				// copy prohibited
	
		int 	startLoop();

	public:
	class Exception_CloseSocket : public std::exception
	{
		private:
		const char *_error_msg;

		public:
		Exception_CloseSocket();
		Exception_CloseSocket(const char *error_msg);
		const char *what() const _NOEXCEPT; // _NOEXCEPT is needed since C++11
	};
};

#endif
