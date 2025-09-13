/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_redirection.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 20:07:07 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/13 07:55:51 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exe_redirection.h"
#include "identifier.h"
#include "exe_signals.h"
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

// char	*f_strip_newline(char *line)
// {
// 	char	*result;
// 	int		linelen;
// 	int		i;

// 	if (!line || !line[0])
// 		return (line);
// 	linelen = ft_strlen(line) - 1;
// 	result = (char *)malloc(linelen + 1);
// 	if (!result)
// 		return (NULL);
// 	// ft_bzero(&result, linelen + 1);
// 	i = -1;
// 	while (line[++i] && line[i] != '\n')
// 		result[i] = line[i];
// 	result[i] = '\0';
// 	free(line);
// 	return (result);
// }

// static int	f_redirect_heredoc(char *delimiter, t_env *env, char **argv)
// {
// 	char			*temp;
// 	char			*stripped_target;
// 	int				exit_value;
// 	size_t			target_len;
// 	bool			expansion_flag;
// 	int				pipe_fd[2];
// 	int				tty;
// 	pid_t			pid;
// 	t_child			*data;

// 	expansion_flag = !is_containing_quotes(delimiter);
// 	stripped_target = remove_quotes(delimiter);
// 	if (!stripped_target)
// 		return (ERROR);
// 	target_len = ft_strlen(stripped_target);
// 	if (pipe(pipe_fd) < 0)
// 		return (perror("heredoc pipe"), ERROR);
// 	tty = open("/dev/tty", O_RDWR);
// 	if (tty < 0)
// 		return (perror("fd tty opening"), FAIL);
// 	pid = fork();
// 	if (pid == 0)
// 	{
// 		close(pipe_fd[IN]);
// 		if (!isatty(STDIN_FILENO))
// 		{
// 			close(STDIN_FILENO);
// 			dup2(tty, STDIN_FILENO);
// 		}
// 		if (!isatty(STDOUT_FILENO))
// 		{
// 			close(STDOUT_FILENO);
// 			dup2(tty, STDOUT_FILENO);
// 		}
// 		close(tty);
// 		// if (dup2(pipe_fd[OUT], STDOUT_FILENO) < 0)
// 		// {
// 		// 	close(pipe_fd[OUT]);
// 		// 	free(stripped_target);
// 		// 	perror("heredoc dup2 write");
// 		// 	mini_exit(ERROR, env);
// 		// }
// 		// close(pipe_fd[OUT]);
// 		rl_catch_signals = 1;
// 		data = f_get_struct();
// 		data->env = env;
// 		data->argv = argv;
// 		data->fd = pipe_fd;
// 		data->target = stripped_target;
// 		data->input = NULL;
// 		f_register_birth_control(env, data);
// 		while (1)
// 		{
// 			// ft_putstr_fd("> ", STDIN_FILENO);
// 			// data->input = get_next_line(STDIN_FILENO);
// 			// tcsetattr(STDIN_FILENO, TCSANOW, &env->orig_term);
// 			data->input = readline("> ");
// 			if (!data->input)
// 			{
// 				free(data->input);
// 				data->input = NULL;
// 				break ;
// 			}
// 			// data->input = f_strip_newline(data->input);
// 			if (ft_strncmp(data->input, data->target, target_len + 1) == 0)
// 			{
// 				free(data->input);
// 				data->input = NULL;
// 				break ;
// 			}
// 			if (expansion_flag)
// 			{
// 				temp = identifier_expand_variables_in_string(data->input, env, true);
// 				free(data->input);
// 				data->input = temp;
// 			}
// 			ft_putendl_fd(data->input, pipe_fd[OUT]);
// 			free(data->input);
// 			data->input = NULL;
// 			rl_on_new_line();
// 			rl_replace_line("", 0);
// 		}
// 		// if (dup2(pipe_fd[OUT], STDOUT_FILENO) < 0)
// 		// {
// 		// 	close(pipe_fd[OUT]);
// 		// 	free(stripped_target);
// 		// 	perror("heredoc dup2 write");
// 		// 	mini_exit(ERROR, env);
// 		// }
// 		close(pipe_fd[OUT]);
// 		f_wash_child(data);
// 		mini_exit(SUCCESS, env);
// 	}
// 	else if (pid > 0)
// 	{
// 		close(tty);
// 		close(pipe_fd[OUT]);
// 		free(stripped_target);
// 		signal(SIGINT, SIG_IGN); // ignore signals
// 		signal(SIGQUIT, SIG_IGN); // ignore signals
// 		exit_value = f_wait_for_child(pid);
// 		f_set_signals(NONINTERACTIVE, env); // restore signals
// 		tcsetattr(STDIN_FILENO, TCSANOW, &env->orig_term);
// 		if (exit_value != SUCCESS)
// 			return (exit_value);
// 		if (dup2(pipe_fd[IN], STDIN_FILENO) < 0)
// 			return (close(pipe_fd[IN]), perror("heredoc dup2 read"), ERROR);
// 		close(pipe_fd[IN]);
// 		return (exit_value);
// 	}
// 	else
// 		return (perror("heredoc fork"), ERROR);
// 	return (SUCCESS);
// }

// static int	f_redirect_heredoc(char *delimiter, t_env *env)
// {
// 	char	*input;
// 	char	*temp;
// 	char	*stripped_target;
// 	int		temp_file;
// 	size_t	target_len;
// 	bool	expansion_flag;

// 	expansion_flag = !is_containing_quotes(delimiter);
// 	temp_file = open(HEREDOC_FILE_NAME, O_WRONLY | O_CREAT | O_TRUNC, 0600);
// 	if (temp_file < 0)
// 		return (perror("heredoc temp file creation"), FAIL);
// 	stripped_target = remove_quotes(delimiter);
// 	if (!stripped_target)
// 		return (ERROR);
// 	target_len = ft_strlen(stripped_target);
// 	// TODO: f_set_signals(INTERACTIVE or special heredoc mode, env);
// 	// signal(SIGINT, SIG_DFL);
// 	// signal(SIGQUIT, SIG_IGN);
// 	while (1)
// 	{
// 		input = readline("> ");
// 		if (!input || ft_strncmp(input, stripped_target, target_len + 1) == 0)
// 		{
// 			free(input);
// 			break ;
// 		}
// 		if (expansion_flag)
// 		{
// 			temp = identifier_expand_variables_in_string(input, env, true);
// 			free(input);
// 			input = temp;
// 		}
// 		ft_putstr_fd_fail_safe(input, temp_file);
// 		write(temp_file, "\n", 1);
// 		free(input);
// 	}
// 	f_set_signals(NONINTERACTIVE, env);
// 	free(stripped_target);
// 	close(temp_file);
// 	temp_file = open(HEREDOC_FILE_NAME, O_RDONLY);
// 	unlink(HEREDOC_FILE_NAME);
// 	if (temp_file < 0)
// 		return (perror("reading heredoc temp file"), ERROR);
// 	else if (dup2(temp_file, STDIN_FILENO) == -1)
// 		return (perror("dup2 redirect heredoc"), ERROR);
// 	close(temp_file);
// 	return(SUCCESS);
// 	// TODO:
// 	/** signal handling for heredocs
// 	 * variable expansion?
// 	 * error checks for write calls?
// 	 * 
// 	 */
// }

static char	*f_heredoc_find_entry(t_ast_node *target, t_env *env)
{
	int	i;

	i = 0;
	while (i < HEREDOC_COUNT)
	{
		if (env->heredoclist[i].redir_ptr == target)
			return (env->heredoclist[i].name);
		++i;
	}
	return (NULL);
}

int	f_redirect_heredoc_new(t_ast_node *redirection, t_env *env)
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

int	f_redirect(t_ast_node *redirections, t_env *env, char **argv)
{
	int	return_value;

	(void)argv; // FIXME: remove later
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
			// return_value = f_redirect_heredoc(redirections->data.redirection.target, env, argv);
			return_value = f_redirect_heredoc_new(redirections, env);
		else
			ft_putstr_fd_fail_safe("invalid redirection type\n", STDERR_FILENO);
		if (return_value != SUCCESS)
			return (return_value);
		redirections = redirections->next;
	}
	return (return_value);
}
