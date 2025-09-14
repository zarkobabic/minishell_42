/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_redirection_heredoc_file_setup.c               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 03:25:16 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/14 11:21:41 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exe_execution.h"
#include "exe_signals.h"
#include "identifier.h"
#include <readline/readline.h>

void	f_heredoc_delete_files(t_env *env)
{
	int	i;

	i = -1;
	while (++i < HEREDOC_COUNT_MAX)
		unlink(env->heredoclist[i].name);
}

static int	f_check_for_signal(void)
{
	if (g_received_signal != -1)
	{
		rl_done = 1;
		rl_replace_line("", false);
		rl_stuff_char('\n');
	}
	return (0);
}

static void	f_heredoc_readline_loop(char *delimiter, int file, t_env *env)
{
	int		expansion_flag;
	int		delim_len;
	char	*input;
	char	*temp;

	expansion_flag = !is_containing_quotes(delimiter);
	delim_len = ft_strlen(delimiter);
	while (1)
	{
		input = readline("> ");
		if (g_received_signal != -1)
			break ;
		if (!input || ft_strncmp(input, delimiter, delim_len + 1) == 0)
			break ;
		if (expansion_flag)
		{
			temp = identifier_expand_variables_in_string(input, env, true);
			free(input);
			input = temp;
		}
		ft_putendl_fd(input, file);
		free(input);
	}
	free(input);
}

int	f_heredoc_read_input(t_env *env, int i, char *delimiter)
{
	int		file;
	char	*stripped_delim;

	file = open(env->heredoclist[i].name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (file < 0)
		return (f_heredoc_delete_files(env), GENERAL_ERR);
	stripped_delim = remove_quotes(delimiter);
	rl_catch_signals = 1;
	f_set_signals(HEREDOC, env);
	rl_event_hook = f_check_for_signal;
	f_heredoc_readline_loop(stripped_delim, file, env);
	rl_event_hook = NULL;
	free(stripped_delim);
	close(file);
	f_set_signals(NONINTERACTIVE, env);
	tcsetattr(STDIN_FILENO, TCSANOW, &env->orig_term);
	if (g_received_signal != -1)
		return (f_heredoc_delete_files(env), GENERAL_ERR);
	return (SUCCESS);
}

void	f_heredoc_populate_entry_file_name(t_env *env, int index)
{
	int		digits;
	int		temp;

	ft_bzero(env->heredoclist[index].name, HEREDOC_NAME_SIZE);
	ft_strlcat(env->heredoclist[index].name, HEREDOC_NAME_PREFIX,
		HEREDOC_NAME_SIZE);
	digits = count_digits(index);
	temp = index;
	env->heredoclist[index].name[sizeof(HEREDOC_NAME_PREFIX) - 1
		+ digits] = '\0';
	while (--digits >= 0)
	{
		env->heredoclist[index].name[sizeof(HEREDOC_NAME_PREFIX) - 1
			+ digits] = '0' + temp % 10;
		temp = temp / 10;
	}
	ft_strlcat(env->heredoclist[index].name, HEREDOC_NAME_SUFFIX,
		HEREDOC_NAME_SIZE);
}
