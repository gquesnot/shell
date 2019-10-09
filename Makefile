NAME = minishell

CFLAGS = -Wall -Wextra -Werror

SRC = main.c \
	cd_builtin.c \
	exit_builtin.c \
	echo_builtin.c \
	set_builtin.c \
	history_builtin.c \
	read_command_line.c \
	term.c \
	key.c \
	signal.c \
	parser2.c \
	pipe.c \
	metachar.c \
	key_cpy.c \
	here_doc.c \
	utils.c \
	key_quote.c \
	bin_hash.c \
	complete.c \
	print_lst.c \
	key_comp.c

LIB = -L ./libft/ -lft -lcurses

OBJ = $(SRC:.c=.o)

all: $(OBJ)
		gcc $(OBJ) -o $(NAME) $(LIB)

clean:
	rm -f $(OBJ)

fclean: clean
		rm -f $(NAME)

re: fclean all
