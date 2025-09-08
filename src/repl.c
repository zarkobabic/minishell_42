/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:39:27 by zbabic            #+#    #+#             */
/*   Updated: 2025/09/08 20:00:16 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "parser.h"
#include "ast_tree_translator.h"
#include "exe_execution.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void	process_input(char *input, t_environ *env)
{
	t_command_line	*tree;
	t_ast_node		*ast_tree;

	tree = parse_tree(input);
	if (!tree)
		return ;
	ast_tree = ast_create_tree(tree);
	env->ast_root = ast_tree;
	ast_tree_print(ast_tree);
	free_parse_tree(tree);
	f_execute_ast(env);
	ast_free_tree(ast_tree);
}

void	mini_exit(int exit_status, t_environ *env)
{
	f_destroy_environ(&env->minienviron, ENV_DELETE_ALL);
	clear_history();
	exit(exit_status);
}

// Dont forget to add -lreadline flag in compilation
// for readline functions to work,
// TODO: delete plsexit here, only for testing purposes
int	main(int argc, char **argv)
{
	char	*input;
	char	*prompt;
	t_environ env;

	if (argc != 1)
		return (printf("ERROR: invalid arguments\n"), 1);
	(void)argv;
	prompt = "minishell> ";
	if(f_build_environ(&env.minienviron) != SUCCESS)
		exit(FAIL);
	printf("Welcome to minishell!\n");
	while (1)
	{
		// f_set_signals_interactive_mode();
		input = readline(prompt);
		if (!input)
		{
			if(isatty(STDIN_FILENO))
				printf("exit\n");
			break ;
		}
		if (ft_strlen(input) == 0)
		{
			free(input);
			continue ;
		}
		add_history(input);
		// f_set_signals_execution_mode();
		process_input(input, &env);
		rl_on_new_line();
		free(input);
	}	
	mini_exit(0, &env);
	return (0);
}
