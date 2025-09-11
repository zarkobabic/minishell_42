/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_redirection.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 20:07:07 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/11 20:47:29 by eberkau          ###   ########.fr       */
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

static int	f_redirect_heredoc(t_ast_node *redirection, t_env *env)
{
	char	*input;
	char	*temp;
	char	*stripped_target;
	int		temp_file;
	size_t	target_len;
	bool	expansion_flag;

	expansion_flag = !is_containing_quotes(redirection->data.redirection.target);
	temp_file = open(HEREDOC_FILE_NAME, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (temp_file < 0)
		return (perror("heredoc temp file creation"), FAIL);
	stripped_target = remove_quotes(redirection->data.redirection.target);
	if (!temp)
		return (ERROR);
	target_len = ft_strlen(stripped_target);
	// TODO: f_set_signals(INTERACTIVE or special heredoc mode, env);
	while (1)
	{
		input = readline("> ");
		if (!input || ft_strncmp(input, stripped_target, target_len + 1) == 0)
		{
			free(input);
			break ;
		}
		if (expansion_flag)
		{
			temp = identifier_expand_variables_in_string(input, env, true);
			free(input);
			input = temp;
		}
		ft_putstr_fd_fail_safe(input, temp_file);
		write(temp_file, "\n", 1);
		free(input);
	}
	// TODO: f_set_signals(NONINTERACTIVE, env);
	free(stripped_target);
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
	 * error checks for write calls?
	 * 
	 */
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
		if (return_value != SUCCESS)
			return (return_value);
		redirections = redirections->next;
	}
	return (return_value);
}
