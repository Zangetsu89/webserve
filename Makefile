# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: lizhang <lizhang@student.codam.nl>           +#+                      #
#                                                    +#+                       #
#    Created: 2023/07/11 12:30:37 by lizhang       #+#    #+#                  #
#    Updated: 2023/07/19 15:30:44 by lizhang       ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

DIRTEST		= DirSettings_tester
SERTEST		= ServerSettings_tester
CONTEST 	= Config_tester
MACTEST		= Macro_tester
CC			= g++
RM			= rm -f
CPPFLAGS	= -Wall -Werror -Wextra -std=c++11
DIRINCLUDE	= -Iinclude/DirSettins.hpp -Iinclude/ConfigMacros.hpp
MACINCLUDE	= -Iinclude/ConfigMacros.hpp 
SERINCLUDE	= -Iinclude/Server.hpp -Iinclude/Port.hpp -Iinclude/ConfigMacros.hpp \
			-Iinclude/DirSettings.hpp
CONINCLUDE	= -Iinclude/Confib.chpp -Iinclude/ConfigMacros.hpp

DIRSRC		= src/config_parse/DirSettings.cpp \
			src/config_parse/ConfigMacros.cpp \
			src/config_parser_tester/DirSettingsTester.cpp

MACSRC		= src/config_parse/ConfigMacros.cpp \
			src/config_parser_tester/MacroTester.cpp

SERSRC		= src/config_parse/Server.cpp src/config_parse/ConfigMacros.cpp \
			src/config_parse/Port.cpp src/config_parse/DirSettings.cpp \
			src/config_parser_tester/ServerTester.cpp 

CONSRC		= src/config_parse/Config.cpp src/config_parse/ConfigMacros.cpp \
			src/config_parser_tester/Config_tester.cpp

DIROBJ		= $(DIRSRC:.cpp=.o)
MACOBJ		= $(MACSRC:.cpp=.o)
SEROBJ		= $(SERSRC:.cpp=.o)
CONOBJ		= $(CONSRC:.cpp=.o)

dirtest:	$(DIRTEST)

sertest:	$(SERTEST)

mactest:	$(MACTEST)

contest:	$(CONTEST)

$(DIRTEST):	$(DIROBJ)
			$(CC) $(CPPFLAGS) $(DIRINCLUDE) $(DIROBJ) -o $(DIRTEST)

$(MACTEST):	$(MACOBJ)
			$(CC) $(CPPFLAGS) $(MACINCLUDE) $(MACOBJ) -o $(MACTEST)

$(SERTEST):	$(SEROBJ)
			$(CC) $(CPPFLAGS) $(SERINCLUDE) $(SEROBJ) -o $(SERTEST)

$(CONTEST): $(CONOBJ)
			$(CC) $(CPPFLAGS) $(CONINCLUDE) $(CONOBJ) -o $(CONTEST)

clean:
			$(RM) $(DIROBJ) $(MACOBJ) $(SEROBJ) $(CONOBJ)

fclean:		clean
			$(RM) $(DIRTEST) $(MACTEST) $(SERTEST) $(CONTEST)

.PHONY:		all clean fclean