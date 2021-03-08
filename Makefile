##
## EPITECH PROJECT, 2021
## B-PSU-400-NCE-4-1-nmobjdump-lorentz-andrea.romeo
## File description:
## Makefile
##

SRC			= 	main.c \
				parser.c \

GCC			=	gcc

GCCFLAGS 	=	-Wall -Wextra -g3 -O0

NAME 		= 	my_nm

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(GCC) -o $(NAME) $(OBJ)

clean:
	rm $(OBJ)

fclean: clean
	rm $(NAME)

re: fclean all

.PHONY: all clean fclean re