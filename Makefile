CC = cc
CFLAGS = -Wall -Wextra -Werror
LFLAGS = -lreadline -lcurses
NAME = minishell
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a
SOURCE_FOLDER = src
OBJECTS_FOLDER = obj
TEMP_FOLDER = .tmp
INCLUDES_FOLDER = inc
INCLUDES = -I$(LIBFT_DIR) -I$(INCLUDES_FOLDER)
REMOVE = rm -rf
DEBUG_ENABLED = -g

MY_C_SOURCES =	repl.c \
				ast/ast_tree_nodes_constructors_commands.c \
				ast/ast_tree_nodes_constructors.c \
				ast/ast_tree_nodes_destructors_commands.c \
				ast/ast_tree_nodes_destructors.c \
				ast/ast_tree_translator_command_elements.c \
				ast/ast_tree_translator_commands.c \
				ast/ast_tree_translator_expressions.c \
				execution_tree/exe_builtins_cd.c \
				execution_tree/exe_builtins_parent.c \
				execution_tree/exe_builtins.c \
				execution_tree/exe_child_process_management.c \
				environment/exe_environment_access.c \
				environment/exe_environment_manipulation.c \
				environment/exe_signals.c \
				execution_tree/exe_execution_tree_leafs.c \
				execution_tree/exe_execution_tree_traversal.c \
				execution_tree/exe_execution_utils.c \
				execution_tree/exe_path_resolution.c \
				execution_tree/exe_pipeline_utils.c \
				execution_tree/exe_pipeline.c \
				execution_tree/exe_simple_cmd_utils.c \
				execution_tree/exe_simple_cmd.c \
				identifier_expansion/identifier_arg_linked_list.c \
				identifier_expansion/identifier_arg_linked_list_cleanup.c \
				identifier_expansion/identifier_bash_sort.c \
				identifier_expansion/identifier_quote_remove.c \
				identifier_expansion/identifier_unquoted_space_split.c \
				identifier_expansion/identifier_unquoted_space_utils.c \
				identifier_expansion/identifier_var_expansion_entry_resolution.c \
				identifier_expansion/identifier_var_expansion_populate.c \
				identifier_expansion/identifier_var_expansion_utils.c \
				identifier_expansion/identifier_var_expansion.c \
				identifier_expansion/identifier_wildcard_curr_dir.c \
				identifier_expansion/identifier_wildcard.c \
				identifier_expansion/identifier_argument.c \
				identifier_expansion/identifier_redirection.c \
				lexer/lexer_get_next_token.c \
				lexer/lexer_lifecycle.c \
				lexer/lexer_operations.c \
				lexer/lexer_utilities.c \
				lexer/token.c \
				parser/parse_tree_nodes_constructors_commands.c \
				parser/parse_tree_nodes_constructors_expressions.c \
				parser/parse_tree_nodes_constructors_redirections.c \
				parser/parse_tree_nodes_destructors_commands.c \
				parser/parse_tree_nodes_destructors_expressions.c \
				parser/parse_tree_nodes_destructors_redirections.c \
				parser/parse_tree_parse_commands.c \
				parser/parse_tree_parse_expressions.c \
				parser/parse_tree_parse_redirections.c \
				redirection/exe_redirection_heredoc_build_array.c \
				redirection/exe_redirection_heredoc_file_setup.c \
				redirection/exe_redirection_utils.c \
				redirection/exe_redirection.c \


MY_OBJECTS = $(addprefix $(OBJECTS_FOLDER)/, $(MY_C_SOURCES:.c=.o))

all: $(LIBFT) $(NAME) | $(TEMP_FOLDER)

$(NAME): $(MY_OBJECTS) $(LIBFT)
	$(CC) $(CFLAGS) $(DEBUG_ENABLED) $^ $(LFLAGS) -o $@

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(OBJECTS_FOLDER)/%.o: $(SOURCE_FOLDER)/%.c | $(OBJECTS_FOLDER)
	$(CC) $(CFLAGS) $(DEBUG_ENABLED) $(INCLUDES) -c $< -o $@

$(OBJECTS_FOLDER):
	@mkdir -p $@
	@mkdir -p $@/ast
	@mkdir -p $@/environment
	@mkdir -p $@/execution_tree
	@mkdir -p $@/identifier_expansion
	@mkdir -p $@/lexer
	@mkdir -p $@/parser
	@mkdir -p $@/redirection

$(TEMP_FOLDER):
	@mkdir -p $@

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	$(REMOVE) $(OBJECTS_FOLDER)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	$(REMOVE) $(TEMP_FOLDER)
	$(REMOVE) $(NAME)

re: fclean all

.PHONY: clean fclean all re
