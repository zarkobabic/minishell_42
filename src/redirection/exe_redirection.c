/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_redirection_modes.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 20:07:07 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/14 10:22:12 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "exe_redirection.h"
#include "identifier.h"

static int	f_redirect_heredoc(t_ast_node *redirection, t_env *env)
{
	int		file;
	char	*filename;

	filename = f_heredoc_find_entry(redirection, env);
	if (!filename)
		return (GENERAL_ERR);
	file = open(filename, O_RDONLY);
	if (file < 0)
		return (perror("heredoc file"), GENERAL_ERR);
	if (dup2(file, STDIN_FILENO) == -1)
		return (close(file), perror("dup2 heredoc"), GENERAL_ERR);
	close(file);
	return (SUCCESS);
}

static int	f_redirect_in(char **target)
{
	int	file;

	file = -1;
	file = open(target[0], O_RDONLY);
	if (file < 0)
		return (perror(target[0]), GENERAL_ERR);
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
		return (perror(target[0]), GENERAL_ERR);
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
		return (GENERAL_ERR);
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
