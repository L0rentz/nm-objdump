##
## EPITECH PROJECT, 2021
## B-PSU-400-NCE-4-1-nmobjdump-lorentz-andrea.romeo
## File description:
## Makefile
##

NM_SRC		= 	nm/main.c \
				nm/check_file.c \
				nm/parse_archive_x64.c \
				nm/parse_archive_x86.c \
				nm/symbols_list.c \
				nm/print_errors.c \
				nm/print_errors_prefix.c \
				nm/symbols_type_x64.c \
				nm/symbols_type_x86.c \
				nm/nm_x64.c \
				nm/nm_x86.c \
				nm/utils.c \

OBJ_SRC		= 	objdump/main.c \
				objdump/check_file.c \
				objdump/parse_archive_x64.c \
				objdump/parse_archive_x86.c \
				objdump/print_errors.c \
				objdump/print_errors_prefix.c \
				objdump/objdump_x64.c \
				objdump/objdump_x86.c \
				objdump/utils.c \

GCC			=	gcc

GCCFLAGS 	=	-Wall -Wextra -I nm/ -I objdump/ -g3 -O0

NM_NAME 	= 	my_nm

OBJ_NAME 	= 	my_objdump

NM_OBJ 		= 	$(NM_SRC:.c=.o)

OBJ_OBJ		= 	$(OBJ_SRC:.c=.o)

all: 		$(NM_NAME) $(OBJ_NAME)

nm:			$(NM_NAME)

objdump:	$(OBJ_NAME)

$(NM_NAME): $(NM_OBJ)
			$(GCC) -o $(NM_NAME) $(NM_OBJ) $(GCCFLAGS)

$(OBJ_NAME): $(OBJ_OBJ)
			$(GCC) -o $(OBJ_NAME) $(OBJ_OBJ) $(GCCFLAGS)

clean:
	rm -f $(OBJ_OBJ)
	rm -f $(NM_OBJ)

fclean: clean
	rm -f $(NM_NAME)
	rm -f $(OBJ_NAME)

re: fclean all

.PHONY: all clean fclean re nm objdump