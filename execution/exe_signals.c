/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_signals.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 18:21:56 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/06 20:23:46 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exe_execution.h"
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

static void	f_signal_newline(int signal)
{
	(void)signal;
	rl_on_new_line();
}

void	f_set_signals_execution_mode(void)
{
	struct sigaction	action_config;

	ft_bzero(&action_config, sizeof(action_config));
	action_config.sa_handler = f_signal_newline;
	sigaction(SIGINT, &action_config, NULL);
	sigaction(SIGQUIT, &action_config, NULL);
}

static void	f_signal_reset_prompt(int signal)
{
	(void)signal;
	write(STDOUT_FILENO, "^C\n", 3);
	rl_on_new_line();
	// rl_replace_line("", FALSE);
	rl_redisplay();
}

// void	f_ignore_sigquit(void)
// {
// 	struct sigaction	action_config_sigquit;

// 	ft_bzero(&action_config_sigquit, sizeof(action_config_sigquit));
// 	action_config_sigquit.sa_handler = SIG_IGN;
// 	if (sigaction(SIGQUIT, &action_config_sigquit, NULL) == -1)
// 		exit(1);
// }

void	f_set_signals_interactive_mode(void)
{
	struct sigaction	action_config_sigint;
	struct sigaction	action_config_sigquit;

	ft_bzero(&action_config_sigint, sizeof(action_config_sigint));
	ft_bzero(&action_config_sigquit, sizeof(action_config_sigquit));
	action_config_sigint.sa_handler = f_signal_reset_prompt;
	action_config_sigquit.sa_handler = SIG_IGN;
	if (sigaction(SIGINT, &action_config_sigint, NULL) == -1
		|| sigaction(SIGQUIT, &action_config_sigquit, NULL) == -1)
		exit(1);
}
