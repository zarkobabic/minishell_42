CC = cc
CFLAGS = -Wall -Wextra -Werror
LFLAGS = -L/opt/homebrew/opt/readline/lib -lhistory -lreadline 
NAME = minishell
LIBFT_DIR = libft/
LIBFT = $(LIBFT_DIR)libft.a
SOURCE_FOLDER = src/
OBJECTS_FOLDER = obj/
INCLUDES_FOLDER = $(SOURCE_FOLDER)
INCLUDES = -I$(LIBFT_DIR) -I$(INCLUDES_FOLDER)
REMOVE = rm -rf
DEBUG_ENABLED = -g

MY_C_SOURCES =	lexer_lifecycle.c \
				lexer_operations.c \
				parser.c \
				repl.c \
				token_composite.c \
				token.c \

MY_OBJECTS = $(addprefix $(OBJECTS_FOLDER), $(MY_C_SOURCES:.c=.o))

all: $(LIBFT) $(NAME)

$(NAME): $(MY_OBJECTS) $(LIBFT)
	$(CC) $(CFLAGS) $(LFLAGS) $(DEBUG_ENABLED) $^ -o $@

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