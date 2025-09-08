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

MY_C_SOURCES =	ast_tree_nodes_constructors_commands.c \
				ast_tree_nodes_constructors.c \
				ast_tree_nodes_destructors_commands.c \
				ast_tree_nodes_destructors.c \
				ast_tree_print.c \
				ast_tree_translator_command_elements.c \
				ast_tree_translator_commands.c \
				ast_tree_translator_expressions.c \
				lexer_get_next_token.c \
				lexer_lifecycle.c \
				lexer_operations.c \
				lexer_utilities.c \
				parse_tree_nodes_constructors_commands.c \
				parse_tree_nodes_constructors_expressions.c \
				parse_tree_nodes_constructors_redirections.c \
				parse_tree_nodes_destructors_commands.c \
				parse_tree_nodes_destructors_expressions.c \
				parse_tree_nodes_destructors_redirections.c \
				parse_tree_parse_commands.c \
				parse_tree_parse_expressions.c \
				parse_tree_parse_redirections.c \
				parse_tree_print.c \
				repl.c \
				token_print.c \
				token.c \
				exe_builtins.c \
				exe_draft.c \
				exe_pipeline.c \
				exe_simple_command.c \
				exe_environment_access.c \
				exe_environment_manipulation.c \
				exe_builtins_cd.c \

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
