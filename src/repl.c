/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:39:27 by zbabic            #+#    #+#             */
/*   Updated: 2025/09/14 12:55:26 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast_tree_translator.h"
#include "exe_execution.h"
#include "exe_signals.h"
#include "lexer.h"
#include "parser.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>

volatile int	g_received_signal = -1;

static void	process_input(char *input, t_env *env)
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
	free_parse_tree(tree);
	f_execute_ast(env);
	ast_free_tree(ast_tree);
}

static int	f_init_shell(t_env *env)
{
	ft_bzero(env, sizeof(*env));
	if (f_environ_build(&env->minienviron) != SUCCESS)
		exit(GENERAL_ERR);
	env->old_exit_value = 0;
	tcgetattr(STDIN_FILENO, &env->orig_term);
	f_set_signals(NONINTERACTIVE, env);
	return (SUCCESS);
}

static int	f_handle_input(char *input, t_env *env, int is_terminal)
{
	if (!input)
	{
		if (is_terminal)
			write(STDERR_FILENO, "exit\n", 5);
		return (0);
	}
	if (ft_strlen(input) == 0)
	{
		free(input);
		return (1);
	}
	add_history(input);
	rl_catch_signals = 0;
	if (ft_strnstr(input, "/minishell", 11))
		f_set_signals(SILENT, env);
	else
		f_set_signals(NONINTERACTIVE, env);
	process_input(input, env);
	free(input);
	rl_on_new_line();
	return (1);
}

static void	f_shell_loop(t_env *env, int is_terminal)
{
	char	*input;
	int		continue_loop;

	while (1)
	{
		tcsetattr(STDIN_FILENO, TCSANOW, &env->orig_term);
		rl_catch_signals = 1;
		if (is_terminal)
			f_set_signals(INTERACTIVE, env);
		input = readline(PROMPT);
		continue_loop = f_handle_input(input, env, is_terminal);
		if (!continue_loop)
			break ;
	}
}

// Needs -lreadline flag in compilation and 
// -lcurses flag in linking process
// for readline functions to work
int	main(int argc, char **argv)
{
	t_env	env;
	int		is_terminal;

	if (argc != 1)
		return (printf("ERROR: invalid arguments\n"), 1);
	(void)argv;
	if (f_init_shell(&env) != SUCCESS)
		return (1);
	is_terminal = isatty(STDIN_FILENO);
	f_shell_loop(&env, is_terminal);
	mini_exit(0, &env);
	return (0);
}
