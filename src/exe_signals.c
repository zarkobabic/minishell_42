/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_signals.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 18:21:56 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/09 04:03:53 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exe_execution.h"
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

#define MSG_SIGQUIT "^\\Quit: 3\n"
#define MSG_SIGINT "^C\n"

static void	f_signal_noninteractive_sigquit(int signo)
{
	// g_received_signal = signo;
	(void)signo;
	if (isatty(STDOUT_FILENO)) // FIXME: check condition
		write(STDERR_FILENO, MSG_SIGQUIT, sizeof(MSG_SIGQUIT));
	// rl_on_new_line();
	// rl_replace_line("", FALSE);
	// rl_redisplay();
}

// FIXME: test, if behavior differs in interactive vs noninteractive; if not, remove this function
static void	f_signal_noninteractive_sigint(int signo)
{
	// g_received_signal = signo;
	(void)signo;
	if (isatty(STDOUT_FILENO)) // FIXME: check condition
		write(STDERR_FILENO, MSG_SIGINT, sizeof(MSG_SIGINT));
	// rl_on_new_line();
	// rl_replace_line("", FALSE);
	// rl_redisplay();
}

void	f_set_signals_execution_mode(void)
{
	struct sigaction	act_conf_sigint;
	struct sigaction	act_conf_sigquit;

	ft_bzero(&act_conf_sigint, sizeof(act_conf_sigint));
	ft_bzero(&act_conf_sigquit, sizeof(act_conf_sigquit));
	act_conf_sigint.sa_handler = f_signal_noninteractive_sigint;
	act_conf_sigquit.sa_handler = f_signal_noninteractive_sigquit;
	if (sigaction(SIGINT, &act_conf_sigint, NULL) == -1
		|| sigaction(SIGQUIT, &act_conf_sigquit, NULL) == -1)
		exit(1); // FIXME: more cleanup
}

static void	f_signal_reset_prompt(int signo)
{
	// g_received_signal = signo;
	(void)signo;
	if (isatty(STDIN_FILENO))
		write(STDERR_FILENO, MSG_SIGINT, sizeof(MSG_SIGINT));
	rl_on_new_line();
	rl_replace_line("", FALSE);
	rl_redisplay();
}

void	f_set_signals_interactive_mode(void)
{
	struct sigaction	act_conf_sigint;
	struct sigaction	act_conf_sigquit;

	ft_bzero(&act_conf_sigint, sizeof(act_conf_sigint));
	ft_bzero(&act_conf_sigquit, sizeof(act_conf_sigquit));
	act_conf_sigint.sa_handler = f_signal_reset_prompt;
	act_conf_sigquit.sa_handler = SIG_IGN;
	if (sigaction(SIGINT, &act_conf_sigint, NULL) == -1
		|| sigaction(SIGQUIT, &act_conf_sigquit, NULL) == -1)
		exit(1); // FIXME: more cleanup
}
