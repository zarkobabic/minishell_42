CC = cc
CFLAGS = -Wall -Wextra -Werror
LFLAGS = -lreadline -lcurses
NAME = minishell
LIBFT_DIR = libft/
LIBFT = $(LIBFT_DIR)libft.a
SOURCE_FOLDER = src/
OBJECTS_FOLDER = obj/
INCLUDES_FOLDER = $(SOURCE_FOLDER)
INCLUDES = -I$(LIBFT_DIR) -I$(INCLUDES_FOLDER)
REMOVE = rm -rf
DEBUG_ENABLED = -g

MY_C_SOURCES =	lexer_get_next_token.c \
				lexer_lifecycle.c \
				lexer_operations.c \
				lexer_utilities.c \
				parse_tree_nodes_constructors_first.c \
				parse_tree_nodes_constructors_second.c \
				parse_tree_nodes_constructors_third.c \
				parse_tree_nodes_destructors_commands.c \
				parse_tree_nodes_destructors_operators.c \
				parse_tree_nodes_destructors_redirections.c \
				parse_tree_print.c \
				parser.c \
				repl.c \
				token.c \

MY_OBJECTS = $(addprefix $(OBJECTS_FOLDER), $(MY_C_SOURCES:.c=.o))

all: $(LIBFT) $(NAME)

$(NAME): $(MY_OBJECTS) $(LIBFT)
	$(CC) $(CFLAGS) $(DEBUG_ENABLED) $^ $(LFLAGS) -o $@

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(OBJECTS_FOLDER)%.o: $(SOURCE_FOLDER)%.c
	@mkdir -p $(OBJECTS_FOLDER)
	$(CC) $(CFLAGS) $(DEBUG_ENABLED) $(INCLUDES) -c $< -o $@

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	$(REMOVE) $(OBJECTS_FOLDER)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	$(REMOVE) $(NAME)

re: fclean all

.PHONY: clean fclean all re
