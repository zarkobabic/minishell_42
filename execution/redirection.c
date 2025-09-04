/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 20:07:07 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/04 21:03:18 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include "../src/ast_tree_structures.h"

static int	f_redirect_in(t_ast_node *redirection)
{
	int	file;

	file = -1;
	file = open(redirection->data.redirection.target, O_RDONLY);
	if (file < 0)
		return (perror(redirection->data.redirection.target), FAIL);
	else if (dup2(file, STDIN_FILENO) == -1)
		return (perror("dup2"), ERROR);
	close(file);
	return (SUCCESS);
}

static int	f_redirect_out(t_ast_node *redirection)
{
	int	file;

	file = -1;
	file = open(redirection->data.redirection.target,
		O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (file < 0)
		return (perror(redirection->data.redirection.target), FAIL);
	else if (dup2(file, STDOUT_FILENO) == -1)
		return (perror("dup2"), ERROR);
	close(file);
	return (SUCCESS);
}

static int	f_redirect_append(t_ast_node *redirection)
{
	int	file;

	file = -1;
	file = open(redirection->data.redirection.target,
		O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (file < 0)
		return (perror(redirection->data.redirection.target), FAIL);
	else if (dup2(file, STDOUT_FILENO) == -1)
		return (perror("dup2"), ERROR);
	close(file);
	return (SUCCESS);
}

static int	f_redirect_heredoc(t_ast_node *redirection)
{
	// TODO
}

int	f_redirect(t_ast_node *redirections)
{
	int	return_value;

	if (!redirections)
		return (SUCCESS);
	return_value = SUCCESS;
	while (redirections)
	{
		if (redirections->data.redirection.type == TYPE_REDIRECT_IN)
			return_value = f_redirect_in(redirections);
		else if (redirections->data.redirection.type == TYPE_REDIRECT_OUT)
			return_value = f_redirect_out(redirections);
		else if (redirections->data.redirection.type == TYPE_REDIRECT_APPEND)
			return_value = f_redirect_append(redirections);
		else if (redirections->data.redirection.type == TYPE_REDIRECT_HEREDOC)
			return_value = f_redirect_heredoc(redirections);
		if (return_value != SUCCESS)
			return (return_value);
		redirections = redirections->next;
	}
	return (return_value);
}
