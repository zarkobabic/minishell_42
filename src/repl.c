/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:39:27 by zbabic            #+#    #+#             */
/*   Updated: 2025/07/31 04:26:27 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "parser.h"
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

	tree = parse_tree(input);
	parse_tree_print(tree);
	free_parse_tree(tree);
}

// Dont forget to add -lreadline flag in compilation
// for readline functions to work
int	main(void)
{
	char	*input;
	char	*prompt;

	prompt = "minishell> ";
	printf("Welcome to minishell!\n");
	while (1)
	{
		input = readline(prompt);
		if (!input)
		{
			printf("See ya!");
			break ;
		}
		if (ft_strncmp(input, "plsexit", 7) == 0) // TODO: delete this, only for parsing testing purposes
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
