/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 20:07:07 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/05 15:30:58 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include "../src/ast_tree_structures.h"
#include <readline/readline.h>

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
	char	*input;
	int		temp_file;
	size_t	target_len;
	size_t	input_len;

	temp_file = open("/tmp/ez_guys_minishell_tempheredocfile.txt",
		O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (temp_file < 0)
		return (perror("heredoc temp file creation"), FAIL);
	target_len = 1 + ft_strlen(redirection->data.redirection.target);
	while (1)
	{
		input = readline("> ");
		if (!input || ft_strncmp(input, redirection->data.redirection.target, target_len) == 0)
		{
			free(input);
			break ;
		}
		input_len = ft_strlen(input);
		write(temp_file, input, input_len);
		write(temp_file, "\n", 1);
		free(input);
	}
	close(temp_file);
	temp_file = open("/tmp/ez_guys_minishell_tempheredocfile.txt", O_RDONLY);
	if (temp_file < 0)
		return (perror("heredoc temp file reading"), ERROR);
	else if (dup2(temp_file, STDIN_FILENO) == -1)
		return (perror("dup2"), ERROR);
	close(temp_file);
	unlink("/tmp/ez_guys_minishell_tempheredocfile.txt");
	return(SUCCESS);
	// TODO:
	/** signal handling for heredocs
	 * variable expansion?
	 * eroor checks for write calls?
	 * 
	 */
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
