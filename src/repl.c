/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:39:27 by zbabic            #+#    #+#             */
/*   Updated: 2025/07/11 17:23:28 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/history.h>
#include <readline/readline.h>

void	clear_history(void)
{
	rl_clear_history();
	printf("Cleanup complete. See ya!\n");
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
		if (strlen(input) == 0)
		{
			free(input);
			continue ;
		}
		add_history(input);
		//process_command(input);
		rl_on_new_line();
		free(input);
	}
	clear_history();
	return (0);
}
