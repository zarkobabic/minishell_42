/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:39:27 by zbabic            #+#    #+#             */
/*   Updated: 2025/09/14 03:52:51 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "parser.h"
#include "ast_tree_translator.h"
#include "exe_execution.h"
#include "exe_signals.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>

int	g_received_signal = -1;

void	process_input(char *input, t_env *env)
{
	t_command_line	*tree;
	t_ast_node		*ast_tree;

	tree = parse_tree(input);
	if (!tree)
	{
		env->old_exit_value = 2;
		return ;
	}
	ast_tree = ast_create_tree(tree);
	env->ast_root = ast_tree;
	//ast_tree_print(ast_tree);
	free_parse_tree(tree);
	f_execute_ast(env);
	ast_free_tree(ast_tree);
}

void	mini_exit(int exit_status, t_env *env)
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
	int		is_terminal;
	t_env	env;

	if (argc != 1)
		return (printf("ERROR: invalid arguments\n"), 1);
	(void)argv;
	ft_bzero(&env, sizeof(env));
	if(f_build_environ(&env.minienviron) != SUCCESS)
		exit(GENERAL);
	env.old_exit_value = 0;
	is_terminal = isatty(STDIN_FILENO);
	tcgetattr(STDIN_FILENO, &env.orig_term);
	f_set_signals(NONINTERACTIVE, &env);
	//printf("Welcome to minishell!\n");
	while (1)
	{
		tcsetattr(STDIN_FILENO, TCSANOW, &env.orig_term);
		rl_catch_signals = 1;
		if (is_terminal)
			f_set_signals(INTERACTIVE, &env);
		input = readline(PROMPT);
		if (!input)
		{
			if(is_terminal)
				write(STDERR_FILENO, "exit\n", 5);
			break ;
		}
		if (ft_strlen(input) == 0)
		{
			free(input);
			continue ;
		}
		add_history(input);
		rl_catch_signals = 0;
		if (ft_strnstr(input, "/minishell", 11))
			f_set_signals(SILENT, &env);
		else
			f_set_signals(NONINTERACTIVE, &env);
		
		process_input(input, &env);
		free(input);
		rl_on_new_line();
	}
	mini_exit(0, &env);
	return (0);
}
