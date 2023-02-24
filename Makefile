NAME		=	container_test

SRC			=	main.cpp \

OBJ			=	$(SRC:.cpp=.o)

CC			=	c++

RM			=	rm -f

CFLAGS		=	-Wall -Wextra -Werror -std=c++98

%.o:%.c
			$(CC) $(CFLAGS) -c $< -o $@

$(NAME):	$(OBJ)
			$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

all:		 $(NAME)

clean:
			${RM} $(OBJ)

fclean:		clean
			${RM} $(NAME) ${OBJ} ./mine.txt ./real.txt

re:			fclean all

.PHONY:		all clean fclean re
