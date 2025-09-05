/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:39:27 by zbabic            #+#    #+#             */
/*   Updated: 2025/09/05 22:37:14 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "parser.h"
#include "ast_tree_translator.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void	clear_history_now(void)
{
	clear_history();
	printf("Cleanup complete. See ya!\n");
}

void	process_input(char *input)
{
	t_command_line	*tree;
	t_ast_node		*ast_tree;

	tree = parse_tree(input);
	if (!tree)
		return ;
	ast_tree = ast_create_tree(tree);
	ast_tree_print(ast_tree);
	free_parse_tree(tree);
	// f_execute_ast(ast_tree);
	ast_free_tree(ast_tree);
}

// Dont forget to add -lreadline flag in compilation
// for readline functions to work,
// TODO: delete plsexit here, only for testing purposes
int	main(char **argv, int argc)
{
	char	*input;
	char	*prompt;

	if (argc != 1)
		return (printf("ERROR: invalid arguments\n"), 1);
	(void)argv;
	prompt = "minishell> ";
	printf("Welcome to minishell!\n");
	while (1)
	{
		input = readline(prompt);
		if (!input)
			break ;
		if (ft_strncmp(input, "plsexit", 7) == 0)
			return (free(input), clear_history_now(), 0);
		if (ft_strlen(input) == 0)
		{
			free(input);
			continue ;
		}
		add_history(input);
		process_input(input);
		rl_on_new_line();
		free(input);
	}
	clear_history_now();
	return (0);
}
