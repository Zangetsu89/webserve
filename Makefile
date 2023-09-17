NAME			=	webserv
CC				=	c++
CFLAGS			=	-Wall -Wextra -Werror -std=c++17 -g #-fsanitize=address
RM				=	rm -f
DIR_HEADER		= 	include
DIR_OBJ			=	obj
DIR_SRC			=	src

#cpp files (add files for main. you can keep your main.cpp and Makefile in your work directory without writing in this file)
SRCS_MAIN		:=	main.cpp
SRCS_ALL		:=	$(DIR_SRC)/WebservCli.cpp		\
					$(DIR_SRC)/util.cpp				\
					$(DIR_SRC)/status.cpp				\
					$(DIR_SRC)/SocketListen.cpp		\
					$(DIR_SRC)/SocketConnect.cpp		\
					$(DIR_SRC)/KqueueLoop.cpp			\
					$(DIR_SRC)/Request.cpp			\
					$(DIR_SRC)/RequestHeader.cpp		\
					$(DIR_SRC)/Response.cpp		\
					$(DIR_SRC)/DirSettings.cpp		\
					$(DIR_SRC)/ConfigMacros.cpp	\
					$(DIR_SRC)/Config.cpp			\
					$(DIR_SRC)/Server.cpp			\
					$(DIR_SRC)/CgiHandler.cpp

SRCS			:=	$(SRCS_MAIN) $(SRCS_ALL) 

HEADERS			=	$(wildcard $(DIR_HEADER)/*.hpp)

OBJS_MAIN		= $(addprefix $(DIR_OBJ)/, $(patsubst %.cpp, %.o , $(SRCS_MAIN)))
OBJS_ALL		= $(patsubst $(DIR_SRC)/%, $(DIR_OBJ)/% , $(patsubst %.cpp, %.o , $(SRCS_ALL)))


OBJS = $(OBJS_ALL) $(OBJS_MAIN)

all:	$(NAME)

$(NAME): $(DIR_HEADER) $(DIR_OBJ) $(OBJS) $(HEADERS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)


$(DIR_OBJ)/%.o:	%.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(DIR_OBJ)/%.o:	$(DIR_SRC)/%.cpp $(HEADERS)
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