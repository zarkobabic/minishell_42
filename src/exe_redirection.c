/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_redirection.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 20:07:07 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/11 05:17:05 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exe_redirection.h"
#include "identifier.h"

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

	expanded_result = identifier_process_redirection(redirection, &arbitrary_argc, env);
	if (!expanded_result)
		return (NULL);
	if (arbitrary_argc > 1)
		return (free_split(expanded_result), printf("minishell: %s: ambiguous redirection\n", redirection->data.redirection.target), NULL);
	else
		return (expanded_result);
}

static int	f_redirect_in(t_ast_node *redirection)
{
	int	file;
	// int	var_expansion;

	file = -1;
	// var_expansion = f_expand_vars(&redirection->data.redirection.target);
	// if (var_expansion != SUCCESS)
	// 	return (var_expansion);
	file = open(redirection->data.redirection.target, O_RDONLY);
	if (file < 0)
		return (perror(redirection->data.redirection.target), FAIL);
	else if (dup2(file, STDIN_FILENO) == -1)
		return (perror("dup2 redirect in"), ERROR);
	close(file);
	return (SUCCESS);
}

static int	f_redirect_out(t_ast_node *redirection, t_env *env)
{
	int		file;
	char	**expanded_target;

	(void)env;
	file = -1;
	expanded_target = f_expand_vars(redirection, env);
	if (!expanded_target)
		return (FAIL);
	file = open(expanded_target[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	free_split(expanded_target);
	if (file < 0)
		return (perror(expanded_target[0]), FAIL);
	else if (dup2(file, STDOUT_FILENO) == -1)
		return (perror("dup2 redirect out"), ERROR);
	close(file);
	return (SUCCESS);
}

static int	f_redirect_append(t_ast_node *redirection)
{
	int	file;

	file = -1;
	// if (f_expand_vars(&redirection->data.redirection.target) != SUCCESS)
	// 	return (FAIL);
	file = open(redirection->data.redirection.target,
		O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (file < 0)
		return (perror(redirection->data.redirection.target), FAIL);
	else if (dup2(file, STDOUT_FILENO) == -1)
		return (perror("dup2 redirect append"), ERROR);
	close(file);
	return (SUCCESS);
}

static int	f_redirect_heredoc(t_ast_node *redirection)
{
	char	*input;
	int		temp_file;
	// bool	expansion_flag;
	size_t	target_len;

	// expansion_flag = !is_containing_quotes(redirection->data.redirection.target);
	temp_file = open(HEREDOC_FILE_NAME, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (temp_file < 0)
		return (perror("heredoc temp file creation"), FAIL);
	// redirection->data.redirection.target = remove_quotes(redirection->data.redirection.target);
	target_len = ft_strlen(redirection->data.redirection.target);
	while (1)
	{
		input = readline("> ");
		if (!input || ft_strncmp(input, redirection->data.redirection.target, target_len + 1) == 0)
		{
			free(input);
			break ;
		}
		// if (expansion_flag)
		// 	input = identifier_expand_variables_in_string(input);
		// input_len = ft_strlen(input);
		// write(temp_file, input, input_len);
		ft_putstr_fd_fail_safe(input, temp_file);
		write(temp_file, "\n", 1);
		free(input);
	}
	close(temp_file);
	temp_file = open(HEREDOC_FILE_NAME, O_RDONLY);
	unlink(HEREDOC_FILE_NAME);
	if (temp_file < 0)
		return (perror("reading heredoc temp file"), ERROR);
	else if (dup2(temp_file, STDIN_FILENO) == -1)
		return (perror("dup2 redirect heredoc"), ERROR);
	close(temp_file);
	return(SUCCESS);
	// TODO:
	/** signal handling for heredocs
	 * variable expansion?
	 * eroor checks for write calls?
	 * 
	 */
}

int	f_redirect(t_ast_node *redirections, t_env *env)
{
	int	return_value;

	if (!redirections)
		return (SUCCESS);
	// TODO: variable expansion
	return_value = SUCCESS;
	while (redirections)
	{
		if (redirections->data.redirection.type == TYPE_REDIRECT_IN)
			return_value = f_redirect_in(redirections/* , env */);
		else if (redirections->data.redirection.type == TYPE_REDIRECT_OUT)
			return_value = f_redirect_out(redirections, env);
		else if (redirections->data.redirection.type == TYPE_REDIRECT_APPEND)
			return_value = f_redirect_append(redirections/* , env */);
		else if (redirections->data.redirection.type == TYPE_REDIRECT_HEREDOC)
			return_value = f_redirect_heredoc(redirections/* , env */);
		if (return_value != SUCCESS)
			return (return_value);
		redirections = redirections->next;
	}
	return (return_value);
}
