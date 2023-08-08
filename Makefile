# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: lizhang <lizhang@student.codam.nl>           +#+                      #
#                                                    +#+                       #
#    Created: 2023/07/11 12:30:37 by lizhang       #+#    #+#                  #
#    Updated: 2023/08/08 14:55:01 by lizhang       ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

DIRTEST		= DirSettings_tester
SERTEST		= ServerSettings_tester
CONTEST 	= Config_tester
MACTEST		= Macro_tester
CHETEST		= Checker_tester
CC			= g++
RM			= rm -f
CPPFLAGS	= -Wall -Werror -Wextra -std=c++11
DIRINCLUDE	= -Iinclude/DirSettins.hpp -Iinclude/ConfigMacros.hpp
MACINCLUDE	= -Iinclude/ConfigMacros.hpp 
SERINCLUDE	= -Iinclude/Server.hpp -Iinclude/Port.hpp -Iinclude/ConfigMacros.hpp \
			-Iinclude/DirSettings.hpp -Iinclude/SocketListen.hpp
CONINCLUDE	= -Iinclude/Config.hpp -Iinclude/ConfigMacros.hpp
CHEINCLUDE	= -Iinclude/ConfigChecker.hpp -Iinclude/ConfigMacros.hpp

DIRSRC		= src/config_parse/DirSettings.cpp \
			src/config_parse/ConfigMacros.cpp \
			src/config_parser_tester/DirSettingsTester.cpp

MACSRC		= src/config_parse/ConfigMacros.cpp \
			src/config_parser_tester/MacroTester.cpp

SERSRC		= src/config_parse/Server.cpp src/config_parse/ConfigMacros.cpp \
			src/config_parse/DirSettings.cpp src/webserve_kq/SocketListen.cpp\
			src/config_parser_tester/ServerTester.cpp 

CONSRC		= src/config_parse/Server.cpp src/config_parse/ConfigMacros.cpp \
			src/config_parse/DirSettings.cpp src/webserve_kq/SocketListen.cpp\
			src/config_parse/Config.cpp\
			src/config_parser_tester/ConfigTester.cpp 

CHESRC		= src/config_parse/Server.cpp src/config_parse/ConfigMacros.cpp \
			src/config_parse/DirSettings.cpp src/webserve_kq/SocketListen.cpp\
			src/config_parse/Config.cpp\
			src/config-checker/ConfigChecker.cpp \
			src/config-checker_tester/config-checker_tester.cpp

DIROBJ		= $(DIRSRC:.cpp=.o)
MACOBJ		= $(MACSRC:.cpp=.o)
SEROBJ		= $(SERSRC:.cpp=.o)
CONOBJ		= $(CONSRC:.cpp=.o)
CHEOBJ		= $(CHESRC:.cpp=.o)

dirtest:	$(DIRTEST)

sertest:	$(SERTEST)

mactest:	$(MACTEST)

contest:	$(CONTEST)

chetest:	$(CHETEST)

$(DIRTEST):	$(DIROBJ)
			$(CC) $(CPPFLAGS) $(DIRINCLUDE) $(DIROBJ) -o $(DIRTEST)

$(MACTEST):	$(MACOBJ)
			$(CC) $(CPPFLAGS) $(MACINCLUDE) $(MACOBJ) -o $(MACTEST)

$(SERTEST):	$(SEROBJ)
			$(CC) $(CPPFLAGS) $(SERINCLUDE) $(SEROBJ) -o $(SERTEST)

$(CONTEST): $(CONOBJ)
			$(CC) $(CPPFLAGS) $(CONINCLUDE) $(CONOBJ) -o $(CONTEST)

$(CHETEST): $(CHEOBJ)
			$(CC) $(CPPFLAGS) $(CHEINCLUDE) $(CHEOBJ) -o $(CHETEST)

clean:
			$(RM) $(DIROBJ) $(MACOBJ) $(SEROBJ) $(CONOBJ)

fclean:		clean
			$(RM) $(DIRTEST) $(MACTEST) $(SERTEST) $(CONTEST)

.PHONY:		all clean fclean