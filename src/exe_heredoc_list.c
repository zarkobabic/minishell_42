/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_heredoc_list.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 03:25:16 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/13 09:30:49 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exe_execution.h"
#include "exe_signals.h"

void	f_delete_heredoc_files(t_env *env)
{
	int	i;

	i = -1;
	while(++i < HEREDOC_COUNT)
		unlink(env->heredoclist[i].name);
}

int	f_heredoc_read_input(t_env *env, int i, char *delimiter)
{
	int		file;
	int		delim_len;
	int		expansion_flag;
	char	*stripped_delim;
	char	*input;
	char	*temp;

	file = open(env->heredoclist[i].name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (file < 0)
		return (f_delete_heredoc_files(env), GENERAL);
	expansion_flag = !is_containing_quotes(delimiter);
	stripped_delim = remove_quotes(delimiter);
	delim_len = ft_strlen(stripped_delim);
	f_set_signals(SILENT, env);
	rl_catch_signals = 1;
	while (1)
	{
		input = readline("> ");
		if (g_received_signal != -1)
		{
			// free(input);
			free(stripped_delim);
			close(file);
			f_set_signals(NONINTERACTIVE, env);
			f_delete_heredoc_files(env);
			return (GENERAL);
		}
		if (!input || ft_strncmp(input, stripped_delim, delim_len + 1) == 0)
		{
			free(input);
			break ;
		}
		if(expansion_flag)
		{
			temp = identifier_expand_variables_in_string(input, env, true);
			free(input);
			input = temp;
		}
		ft_putendl_fd(input, file);
		free(input);
	}
	free(stripped_delim);
	close(file);
	f_set_signals(NONINTERACTIVE, env);
	return (SUCCESS);
}

void	f_heredoc_populate_entry_file_name(t_env *env, int i)
{
	char	*nbr;

	ft_strlcat(env->heredoclist[i].name, HEREDOC_NAME_PREFIX, HEREDOC_NAME_SIZE);
	nbr = ft_itoa(i); // FIXME: maybe change to not allocate
	ft_strlcat(env->heredoclist[i].name, nbr, HEREDOC_NAME_SIZE);
	free(nbr);
	ft_strlcat(env->heredoclist[i].name, HEREDOC_NAME_SUFFIX, HEREDOC_NAME_SIZE);
}

int	f_setup_heredocs(t_ast_node *cmd, t_env *env)
{
	static int	hrdc_counter = 0;
	// int			cmd_counter;
	t_ast_node *temp_redir;

	if (!cmd)
		return (GENERAL);
	// cmd_counter = 0;
	hrdc_counter = hrdc_counter % HEREDOC_COUNT;
	// while (cmd)
	// {
		if (cmd->type == AST_SUBSHELL)
			temp_redir = cmd->data.subshell.redirections;
		else
			temp_redir = cmd->data.command.redirections;
		while (temp_redir)
		{
			if (temp_redir->data.redirection.type == TYPE_REDIRECT_HEREDOC)
			{
				env->heredoclist[hrdc_counter].redir_ptr = temp_redir;
				f_heredoc_populate_entry_file_name(env, hrdc_counter);
				if (f_heredoc_read_input(env, hrdc_counter, temp_redir->data.redirection.target) != SUCCESS)
					return (GENERAL);
				++hrdc_counter;
			}
			temp_redir = temp_redir->next;
		}
		// cmd = cmd->next;
		// ++cmd_counter;
	// }
	return (SUCCESS);
}
