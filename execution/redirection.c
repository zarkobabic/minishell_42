/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 20:07:07 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/06 01:58:27 by eberkau          ###   ########.fr       */
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
	bool	expansion_flag;
	size_t	target_len;
	size_t	input_len;

	expansion_flag = !is_containing_quotes(redirection->data.redirection.target);
	temp_file = open("/tmp/ez_guys_minishell_tempheredocfile.txt",
		O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (temp_file < 0)
		return (perror("heredoc temp file creation"), FAIL);
	redirection->data.redirection.target = remove_quotes(redirection->data.redirection.target);
	target_len = ft_strlen(redirection->data.redirection.target);
	while (1)
	{
		input = readline("> ");
		if (!input || ft_strncmp(input, redirection->data.redirection.target, target_len + 1) == 0)
		{
			free(input);
			break ;
		}
		if (expansion_flag)
			input = identifier_expand_variables_in_string(input);
		// input_len = ft_strlen(input);
		// write(temp_file, input, input_len);
		ft_putstr_fd_fail_safe(input, temp_file);
		write(temp_file, "\n", 1);
		free(input);
	}
	close(temp_file);
	temp_file = open("/tmp/ez_guys_minishell_tempheredocfile.txt", O_RDONLY);
	unlink("/tmp/ez_guys_minishell_tempheredocfile.txt");
	if (temp_file < 0)
		return (perror("heredoc temp file reading"), ERROR);
	else if (dup2(temp_file, STDIN_FILENO) == -1)
		return (perror("dup2"), ERROR);
	close(temp_file);
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
	// TODO: variable expansion
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
