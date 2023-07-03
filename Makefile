NAME			=	webserv
CC				=	c++
CFLAGS			=	-Wall -Wextra -Werror -std=c++11 #-fsanitize=address
RM				=	rm -f
DIR_HEADER		= 	include
DIR_OBJ			=	obj
DIR_SRC			=	src

#directory for each work
DIR_CLI			=	webserve_cli
DIR_KQ			=	webserve_kq

#cpp files (add files for main. you can keep your main.cpp and Makefile in your work directory without writing in this file)
SRCS_MAIN		:=	main.cpp
SRCS_CLI		:=	$(DIR_SRC)/$(DIR_CLI)/WebservCli.cpp
SRCS_KQ			:=	$(DIR_SRC)/$(DIR_KQ)/Server.cpp					\
					$(DIR_SRC)/$(DIR_KQ)/SocketListen.cpp			\
					$(DIR_SRC)/$(DIR_KQ)/SocketConnect.cpp

SRCS			:= $(SRCS_MAIN) $(SRCS_CLI) $(SRCS_KQ)

HEADERS			=	$(wildcard $(DIR_HEADER)/*.hpp)

OBJS_MAIN		= $(addprefix $(DIR_OBJ)/, $(patsubst %.cpp, %.o , $(SRCS_MAIN)))
OBJS_CLI		= $(patsubst $(DIR_SRC)/$(DIR_CLI)/%, $(DIR_OBJ)/% , $(patsubst %.cpp, %.o , $(SRCS_CLI)))
OBJS_KQ			= $(patsubst $(DIR_SRC)/$(DIR_KQ)/%, $(DIR_OBJ)/% , $(patsubst %.cpp, %.o , $(SRCS_KQ)))

OBJS = $(OBJS_MAIN) $(OBJS_CLI) $(OBJS_KQ)

all:			$(NAME)

$(NAME): $(DIR_HEADER) $(DIR_OBJ) $(OBJS) $(HEADERS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)


$(DIR_OBJ)/%.o:	%.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(DIR_OBJ)/%.o:	$(DIR_SRC)/$(DIR_CLI)/%.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(DIR_OBJ)/%.o:	$(DIR_SRC)/$(DIR_KQ)/%.cpp $(HEADERS)
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