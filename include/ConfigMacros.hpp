
#ifndef CONFIGMACROS_HPP
# define CONFIGMACROS_HPP

# include <string>
# include <vector>

std::string getContent(std::string file_content, std::string title, size_t start_pos);

std::string getValue(std::string content, std::string title, size_t start_pos);

std::vector<std::string> charSplit(std::string src, char c);

std::vector<std::string> strSplit(std::string src, std::string str);

std::string getBracketContent (std::string name, std::string text, size_t start_pos);

#endif