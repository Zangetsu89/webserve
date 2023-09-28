#ifndef UTIL_H
# define UTIL_H

# include <vector>
# include <map>
# include <iostream>

class Server;

std::string					getOneLine(std::string text);
int							returnError(int err, std::string message);
std::string		            vectorToString(std::vector<char> *data);
std::string 				removeFromBothSide(std::string str, const char	*noneed);
std::string 				removeFromRightSide(std::string str, const char	*noneed);
std::string		            removeWhitespace(std::string str);
std::string		            removeCoronBraket(std::string str);
std::string		            splitString(std::string *data, std::string delimita);
void		                deleteStringEnd(std::string *data, std::string delimita);
std::pair<std::string, std::string> getLabelItem(std::string *line, std::string delimita);
void						addString_toVectorChar(std::vector<char> *vect, std::string str);
std::vector<std::string>	makeStrVector(std::string param, std::string delim);

std::string					returnValueByLine(std::string line, std::string title);


// Exception to stop the server (fatal error)
class Exception_StopServer: public std::exception
{
    private:
		const char *_error_msg;

		public:
		Exception_StopServer();
		Exception_StopServer(const char *error_msg);
		const char *what() const noexcept; // noexcept is needed since C++11
};

#endif //UTIL_H
