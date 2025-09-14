/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_signals.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 18:21:56 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/14 09:16:38 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exe_signals.h"

#define MSG_SIGQUIT "Quit: 3\n"
#define MSG_SIGINT "\n"

static void	f_signal_noninteractive_sigquit(int signo)
{
	g_received_signal = BASE_SIGNO + signo;
	write(STDERR_FILENO, MSG_SIGQUIT, sizeof(MSG_SIGQUIT));
}

static void	f_signal_noninteractive_sigint(int signo)
{
	g_received_signal = BASE_SIGNO + signo;
	write(STDERR_FILENO, "\n", 1);
}

static void	f_signal_heredoc(int signo)
{
	g_received_signal = BASE_SIGNO + signo;
}

static void	f_signal_reset_prompt(int signo)
{
	g_received_signal = BASE_SIGNO + signo;
	write(STDERR_FILENO, MSG_SIGINT, sizeof(MSG_SIGINT));
	rl_replace_line("", true);
	rl_on_new_line();
	rl_redisplay();
}

void	f_set_signals(t_sigmode mode, t_env *env)
{
	struct sigaction	act_conf_sigint;
	struct sigaction	act_conf_sigquit;

	ft_bzero(&act_conf_sigint, sizeof(act_conf_sigint));
	ft_bzero(&act_conf_sigquit, sizeof(act_conf_sigquit));
	act_conf_sigint.sa_flags = 0;
	act_conf_sigquit.sa_handler = SIG_IGN;
	if (mode == NONINTERACTIVE)
	{
		act_conf_sigint.sa_handler = f_signal_noninteractive_sigint;
		act_conf_sigquit.sa_handler = f_signal_noninteractive_sigquit;
	}
	else if (mode == INTERACTIVE)
		act_conf_sigint.sa_handler = f_signal_reset_prompt;
	else if (mode == HEREDOC)
		act_conf_sigint.sa_handler = f_signal_heredoc;
	else if (mode == SILENT)
		act_conf_sigint.sa_handler = SIG_IGN;
	if (sigaction(SIGINT, &act_conf_sigint, NULL) == -1
		|| sigaction(SIGQUIT, &act_conf_sigquit, NULL) == -1)
		mini_exit(ERROR, env);
}
