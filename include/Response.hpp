#ifndef RESPONSE_HPP
# define RESPONSE_HPP
# include <string>
# include <vector>
# include <filesystem>
# include <iostream>
# include <fstream>
# include <sys/socket.h>
# include <sys/event.h>

class SocketConnect;
class Request;
class Response
{
	private:
		Request         	*_request;
		SocketConnect   	*_responseSocket;
		std::string			_responseFilePath;
		std::string			_responseContentType;
		int					_responseContentLength;
		std::vector<char>	_responseBody;
		std::vector<char>	_responseHeader;
		std::vector<char>	_responseFullData;
		int					_responseSize;
		int					_responseSent;
		int					_responseFD;
		int					_responseCGIFD;
		int					_responseErrorNum;
		int					_responseStatusNum;
		struct kevent		_responseFileKevent;

	public:
		Response();
		Response(Request *request);
		Response(Response const &src);
		virtual ~Response();
		Response &operator=(Response const &rhs);

		int			getResponseSize() const;
		std::string	getResponseFilePath() const;
		int			getResponseContentLength() const;
		int			getCurrentResponseBodySize() const;
		int			getResponseFD() const;
		int			getResponseCGIFD() const;
		int			getErrorNum() const;
		int			getStatusNum() const;
		SocketConnect   	*getResponseSocket();

		void		setError(int err);
		void		setStatus(int status);
		void		setResponseCGIFD(int fd);
		void		addCtoResponseBody(char c);
		void		addResponseContentLength(int i);
		bool 		setResponseBody(SocketConnect *socket);
		bool		checkRedirectSet();
		void		makeResponseHeader();
		void		makeResponseFullData();
		int			checkResponseStatus();
		void		readResponseSmallFile();
		void		openFD_ResponseFile();
		int 		readFile(int fd);
		void		generateDirectoryListing();
		void		generateRedirectData();
		void		generateErrorBody();
		int			sendResponse(int socket);
};

#endif
