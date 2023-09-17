#ifndef UTIL_H
# define UTIL_H

# include <vector>
# include <map>
# include <iostream>

class Server;

int							returnError(int err, std::string message);
std::string		            vectorToString(std::vector<char> *data);
std::string		            removeWhitespace(std::string str);
std::string		            splitString(std::string *data, std::string delimita);
void		                deleteStringEnd(std::string *data, std::string delimita);
std::pair<std::string, std::string> getLabelItem(std::string *line, std::string delimita);
void						addString_toVectorChar(std::vector<char> *vect, std::string str);
std::vector<std::string>	makeStrVector(std::string param, std::string delim);
#endif //UTIL_H
