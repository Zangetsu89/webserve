# 
#  ____     ___   __  __ // ______    ____   ____ __ __ __  ____ __    __ __
#  || \\   // \\  ||\ ||    | || |    || \\ ||    || || || ||    ||    || ||
#  ||  )) ((   )) ||\\||      ||      ||_// ||==  \\ // || ||==  \\ /\ // ||
#  ||_//   \\_//  || \||      ||      || \\ ||___  \V/  || ||___  \V/\V/  ..
# 
#  only for indivisual test use (the production file is in the top directory)


NAME			=	webserv
CC				=	c++
CFLAGS			=	-Wall -Wextra -Werror -std=c++11 #-fsanitize=address
RM				=	rm -f
DIR_HEADER		= 	../../include
DIR_OBJ			=	obj
#SRCS			:=	$(shell find src -iname "*.cpp")
SRCS			:=	$(wildcard *.cpp)
HEADERS			=	$(wildcard $(DIR_HEADER)/*.hpp)

OBJS			= $(addprefix $(DIR_OBJ)/, $(patsubst %.cpp, %.o , $(SRCS)))

all:			$(NAME)

$(NAME): $(DIR_HEADER) $(DIR_OBJ) $(OBJS) $(HEADERS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(DIR_OBJ)/%.o:	%.cpp $(HEADERS)
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