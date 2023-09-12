#ifndef RESPONSE_HPP
# define RESPONSE_HPP
# include <string>
# include <vector>
# include <filesystem>
# include <iostream>
# include <fstream>

class SocketConnect;
class Request;
class Response
{

	private:
	Request         *_request;
	SocketConnect   *_responseSocket;
	std::string		_responseFilePath;
	int				_responseStatusCode;
	std::string		_responseStatusMessage;
	std::string		_responseContentType;
	int				_responseContentLength;
	std::vector<char>	_responseBody;
	std::string		_responseHeader;

	public:
	Response();
	Response(Request *request);
	Response(Response const &src);
	virtual ~Response();
	Response &operator=(Response const &rhs);

	std::string	getResponseFilePath();
	void		addCtoResponseBody(char c);
	void		addResponseContentLength(int i);
	void		makeResponse(Request *request, SocketConnect *socket);
	void		makeResponseHeader();
	int			checkResponseStatus();
	void		readResponseFile();
	void		generateDirectoryListing();
	void		generateErrorBody();
	void		makeResponseBody();
	void		sendResponseHeader(int socket);
	void		sendResponseBody(int socket);
	std::string filterContentType(std::string filePath);
};

#endif
