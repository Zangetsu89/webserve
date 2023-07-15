#ifndef UTIL_H
# define UTIL_H

# include <vector>
# include <map>
# include <iostream>
# include "Server.hpp"
# include "Request.hpp"

class Server;

std::string		            toString(std::vector<char> *data);
std::string		            removeWhitespace(std::string str);
std::string		            splitString(std::string *data, std::string delimita);
void		                deleteStringEnd(std::string *data, std::string delimita);
std::pair<std::string, std::string> getLabelItem(std::string *line, std::string delimita);
Server                              *findServer(std::string name, std::vector<Server> *list_server);
DirSettings                         *findDirSetting(Request *request, Server *server) ;
bool                                checkMethodAllowed(std::string request_method, DirSettings *setting);
#endif //UTIL_H
