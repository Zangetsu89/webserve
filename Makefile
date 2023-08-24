NAME			=	webserv
CC				=	c++
CFLAGS			=	-Wall -Wextra -Werror -std=c++11 -g #-fsanitize=address
RM				=	rm -f
DIR_HEADER		= 	include
DIR_OBJ			=	obj
DIR_SRC			=	src

#directory for each work
DIR_CLI			=	webserve_cli
DIR_KQ			=	webserve_kq
DIR_PARSE		=	config_parse
DIR_CGI			=	cgi_handler

#cpp files (add files for main. you can keep your main.cpp and Makefile in your work directory without writing in this file)
SRCS_MAIN		:=	main.cpp
SRCS_CLI		:=	$(DIR_SRC)/$(DIR_CLI)/WebservCli.cpp
SRCS_KQ			:=	$(DIR_SRC)/$(DIR_KQ)/util.cpp					\
					$(DIR_SRC)/$(DIR_KQ)/SocketListen.cpp			\
					$(DIR_SRC)/$(DIR_KQ)/SocketConnect.cpp			\
					$(DIR_SRC)/$(DIR_KQ)/KqueueLoop.cpp				\
					$(DIR_SRC)/$(DIR_KQ)/Request.cpp				\
					$(DIR_SRC)/$(DIR_KQ)/RequestHeader.cpp

SRCS_PARSE		:=	$(DIR_SRC)/$(DIR_PARSE)/DirSettings.cpp			\
					$(DIR_SRC)/$(DIR_PARSE)/ConfigMacros.cpp		\
					$(DIR_SRC)/$(DIR_PARSE)/Config.cpp				\
					$(DIR_SRC)/$(DIR_PARSE)/Server.cpp

SRCS_CGI		:=	$(DIR_SRC)/$(DIR_CGI)/CgiHandler.cpp
SRCS			:=	$(SRCS_MAIN) $(SRCS_CGI) $(SRCS_CLI) $(SRCS_KQ) $(SRCS_PARSE)

HEADERS			=	$(wildcard $(DIR_HEADER)/*.hpp)

OBJS_MAIN		= $(addprefix $(DIR_OBJ)/, $(patsubst %.cpp, %.o , $(SRCS_MAIN)))
OBJS_CLI		= $(patsubst $(DIR_SRC)/$(DIR_CLI)/%, $(DIR_OBJ)/% , $(patsubst %.cpp, %.o , $(SRCS_CLI)))
OBJS_KQ			= $(patsubst $(DIR_SRC)/$(DIR_KQ)/%, $(DIR_OBJ)/% , $(patsubst %.cpp, %.o , $(SRCS_KQ)))
OBJS_PARSE		= $(patsubst $(DIR_SRC)/$(DIR_PARSE)/%, $(DIR_OBJ)/% , $(patsubst %.cpp, %.o , $(SRCS_PARSE)))
OBJS_CGI		= $(patsubst $(DIR_SRC)/$(DIR_CGI)/%, $(DIR_OBJ)/% , $(patsubst %.cpp, %.o , $(SRCS_CGI)))

OBJS = $(OBJS_MAIN) $(OBJS_CLI) $(OBJS_KQ) $(OBJS_PARSE) $(OBJS_CGI)

all:			$(NAME)

$(NAME): $(DIR_HEADER) $(DIR_OBJ) $(OBJS) $(HEADERS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)


$(DIR_OBJ)/%.o:	%.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(DIR_OBJ)/%.o:	$(DIR_SRC)/$(DIR_CLI)/%.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(DIR_OBJ)/%.o:	$(DIR_SRC)/$(DIR_KQ)/%.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(DIR_OBJ)/%.o:	$(DIR_SRC)/$(DIR_PARSE)/%.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(DIR_OBJ)/%.o:	$(DIR_SRC)/$(DIR_CGI)/%.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(DIR_HEADER):
	mkdir -p $(DIR_HEADER)

$(DIR_OBJ):
	mkdir -p $(DIR_OBJ)

clean:
	$(RM) $(OBJS)

fclean:		clean
	$(RM) $(NAME)

re:			fclean all

.PHONY:		all clean fclean re