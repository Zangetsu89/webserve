NAME		=	webserv
CC			=	clang++
CFLAGS		=	-Wall -Wextra -Werror -std=c++17 #-fsanitize=address
RM			=	rm -f
SRCS		:=	$(shell find src -iname "*.cpp")
HEADERS		:=	include/WebservCli.hpp
OBJS		= $(SRCS:.cpp=.o)

all:		$(NAME)

$(NAME):	$(OBJS)
			$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o:		%.cpp $(HEADERS)
			$(CC) $(CFLAGS) -c $< -o $@

clean:
			$(RM) $(OBJS)

fclean:		clean
			$(RM) $(NAME)

re:			fclean all

.PHONY:		all clean fclean re