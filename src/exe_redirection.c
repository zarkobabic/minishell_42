/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_redirection.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 20:07:07 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/14 09:17:22 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "exe_redirection.h"
#include "exe_signals.h"
#include "identifier.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <termios.h>

int	f_check_ambiguity(char *target)
{
	char	**target_list;
	int		i;

	i = -1;
	while (target[++i])
	{
		if (target[i] == ' ')
			return (true);
	}
	target_list = get_all_matched_names(target);
	if (!target_list)
		return (ERROR);
	if (target_list[0] && target_list[1])
		return (true);
	i = 0;
	while (target_list[i])
		free(target_list[i++]);
	free(target_list);
	return (false);
}

char	**f_expand_vars(t_ast_node *redirection, t_env *env)
{
	long	arbitrary_argc;
	char	**expanded_result;

	expanded_result = identifier_process_redirection(redirection,
			&arbitrary_argc, env);
	if (!expanded_result)
		return (NULL);
	if (arbitrary_argc > 1)
		return (free_split(expanded_result),
			ft_putstr_fd(redirection->data.redirection.target,
				STDERR_FILENO),
			ft_putstr_fd(": ambiguous redirection\n", STDERR_FILENO),
			NULL);
	else
		return (expanded_result);
}

static int	f_redirect_in(char **target)
{
	int	file;

	file = -1;
	file = open(target[0], O_RDONLY);
	if (file < 0)
		return (perror(target[0]), FAIL);
	else if (dup2(file, STDIN_FILENO) == -1)
		return (close(file), perror("dup2 redirect in"), ERROR);
	close(file);
	return (SUCCESS);
}

static int	f_redirect_out_append(char **target, int mode)
{
	int	file;

	file = -1;
	file = open(target[0], O_WRONLY | O_CREAT | mode, 0644);
	if (file < 0)
		return (perror(target[0]), FAIL);
	else if (dup2(file, STDOUT_FILENO) == -1)
		return (close(file), perror("dup2 redirect out/append"), ERROR);
	close(file);
	return (SUCCESS);
}

static int	f_redirect_simple(t_ast_node *redirection, t_env *env)
{
	char	**expanded_target;
	int		return_value;

	expanded_target = f_expand_vars(redirection, env);
	if (!expanded_target)
		return (FAIL);
	if (redirection->data.redirection.type == TYPE_REDIRECT_IN)
		return_value = f_redirect_in(expanded_target);
	else if (redirection->data.redirection.type == TYPE_REDIRECT_OUT)
		return_value = f_redirect_out_append(expanded_target, O_TRUNC);
	else if (redirection->data.redirection.type == TYPE_REDIRECT_APPEND)
		return_value = f_redirect_out_append(expanded_target, O_APPEND);
	else
		return (ERROR);
	free_split(expanded_target);
	return (return_value);
}

static char	*f_heredoc_find_entry(t_ast_node *target, t_env *env)
{
	int	i;

	i = 0;
	while (i < HEREDOC_COUNT_MAX)
	{
		if (env->heredoclist[i].redir_ptr == target)
			return (env->heredoclist[i].name);
		++i;
	}
	return (NULL);
}

int	f_redirect_heredoc(t_ast_node *redirection, t_env *env)
{
	int		file;
	char	*filename;

	filename = f_heredoc_find_entry(redirection, env);
	if (!filename)
		return (GENERAL);
	file = open(filename, O_RDONLY);
	if (file < 0)
		return (perror("heredoc file"), GENERAL);
	if (dup2(file, STDIN_FILENO) == -1)
		return (close(file), perror("dup2 heredoc"), GENERAL);
	close(file);
	return (SUCCESS);
}

int	f_redirect(t_ast_node *redirections, t_env *env)
{
	int	return_value;

	if (!redirections)
		return (SUCCESS);
	return_value = SUCCESS;
	while (redirections)
	{
		if (redirections->data.redirection.type == TYPE_REDIRECT_IN
			|| redirections->data.redirection.type == TYPE_REDIRECT_OUT
			|| redirections->data.redirection.type == TYPE_REDIRECT_APPEND)
			return_value = f_redirect_simple(redirections, env);
		else if (redirections->data.redirection.type == TYPE_REDIRECT_HEREDOC)
			return_value = f_redirect_heredoc(redirections, env);
		else
			ft_putstr_fd_fail_safe("invalid redirection type\n", STDERR_FILENO);
		if (return_value != SUCCESS)
			return (return_value);
		redirections = redirections->next;
	}
	return (return_value);
}
