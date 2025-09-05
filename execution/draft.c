/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draft.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 22:13:45 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/05 21:01:35 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include "../src/ast_tree_structures.h"

int	f_is_builtin(char *call)
{
	return (!ft_strncmp(call, "echo", 9)
		|| !ft_strncmp(call, "cd", 9)
		|| !ft_strncmp(call, "pwd", 9)
		|| !ft_strncmp(call, "export", 9)
		|| !ft_strncmp(call, "unset", 9)
		|| !ft_strncmp(call, "env", 9)
		|| !ft_strncmp(call, "exit", 9));
}

int	f_execute_cmd(t_ast_node *cmd)
{
	// pid_t	pid;

	// // filter for builtins
	// if (cmd->type == AST_IDENTIFIER
	// 	&& (cmd->data.identifier.string_value == "echo"
	// 	|| cmd->data.identifier.string_value == "cd"
	// 	|| cmd->data.identifier.string_value == "pwd"
	// 	|| cmd->data.identifier.string_value == "export"
	// 	|| cmd->data.identifier.string_value == "unset"
	// 	|| cmd->data.identifier.string_value == "env"
	// 	|| cmd->data.identifier.string_value == "exit"))
	// 	f_execute_builtin(cmd);
	// else
	// pid = fork();
	// if (pid == 0)
	// {
	// 	if (cmd->type == AST_CMD)
	// }
}

int	f_exec_builtin(char **argv, long argc)
{
	if (ft_strncmp(argv[0], "echo", 9) == 0)
		return (f_echo(argv, argc));
	else if (ft_strncmp(argv[0], "cd", 9) == 0)
		return (f_cd(argv, argc));
	else if (ft_strncmp(argv[0], "pwd", 9) == 0)
		return (f_pwd(argv, argc));
	else if (ft_strncmp(argv[0], "export", 9) == 0)
		return (f_export(argv, argc));
	else if (ft_strncmp(argv[0], "unset", 9) == 0)
		return (f_unset(argv, argc));
	else if (ft_strncmp(argv[0], "env", 9) == 0)
		return (f_env(argv, argc));
	else if (ft_strncmp(argv[0], "exit", 9) == 0)
		return (f_exit(argv, argc));
	else
		// TODO: maybe executables here?
	return (ERROR);
}

char **f_buildargv(t_ast_node *args, long argc)
{
	char	**argv;
	long	i;

	if (!args || args->type != AST_IDENTIFIER)
		return (NULL);
	argv = (char **)malloc(argc * sizeof(char *)); // TODO: shallow free that thing!
	if (!argv)
		return (NULL);
	i = 0;
	while (args && i < argc)
	{
		argv[i] = args->data.identifier.string_value;
		args = args->next;
		++i;
	}
	if (args || i != argc)
		return (free(argv), NULL);
	return (argv);
}

int f_exec_simple_cmd(t_ast_node *simcmd)
{
	char	**argv;
	long	argc;
	
	argc = simcmd->data.command.arg_cnt;
	argv = f_buildargv(simcmd->data.command.args, argc);

	if (!ft_strncmp(argv[0], "cd", 9) || !ft_strncmp(argv[0], "export", 9)
		|| !ft_strncmp(argv[0], "unset", 9) || !ft_strncmp(argv[0], "exit", 9))
		f_exec_builtin(argv, argc); // execute without fork
	else
	{
		// TODO: fork and execute
	}
	free(argv); // TODO: check, if viable at this point
}

int	f_exec_subshell(t_ast_node *subshell)
{
	pid_t	pid;
	int		exit_value;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		exit_value = f_redirect(subshell->data.subshell.redirections);
		if (exit_value != SUCCESS)
			exit (exit_value);
		exit_value = f_exec_logic_expr(subshell->data.subshell.logical_expr);
		exit(exit_value);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			exit_value = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			exit_value = 128 + WTERMSIG(status);
	}
	else
		return (perror("fork"), ERROR);
	return (exit_value);
}

int	f_exec_cmd(t_ast_node *cmd)
{
	if (cmd->next != NULL)
		return (ERROR);
	// TODO: variable expansion
	if (cmd->type == AST_CMD)
		return (f_exec_simple_cmd(cmd));
	else if (cmd->type == AST_SUBSHELL)
		return (f_exec_subshell(cmd));
	else
		return (ERROR);
}

void	f_cleanup_pipeline(pid_t *pid_list, int created_processes, int temp_fd)
{
	int	i;

	if (temp_fd != STDIN_FILENO)
		close(temp_fd);
	i = 0;
	while (i < created_processes)
	{
		if (pid_list[i] > 0)
			waitpid(pid_list[i], NULL, 0);
		++i;
	}
	if (pid_list)
		free(pid_list);
}

int	f_exec_pipeline(t_ast_node *pipeline)
{
	if (!pipeline || pipeline->type != AST_PIPELINE
		|| pipeline->data.pipeline.cmd_cnt <= 0 || !pipeline->data.pipeline.cmds)
		return (ERROR);
	if (pipeline->data.pipeline.cmd_cnt == 1)
		return (f_exec_cmd(pipeline->data.pipeline.cmds));
	else
	{
		int		i;
		int		status;
		int		exit_value;
		int		pipe_fds[2];
		int		temp_fd_in;
		pid_t	*pid_list;
		t_ast_node	*temp_cmd;

		pid_list = (pid_t *)malloc(pipeline->data.pipeline.cmd_cnt * sizeof(pid_t));
		if (!pid_list)
			return (perror("malloc"), ERROR);
		temp_cmd = pipeline->data.pipeline.cmds;
		temp_fd_in = STDIN_FILENO;
		exit_value = SUCCESS;
		i = 0;
		while (temp_cmd)
		{
			if (temp_cmd->next)
			{
				if (pipe(pipe_fds) < 0)
				{
					f_cleanup_pipeline(pid_list, i, temp_fd_in);
					return (perror("pipe"), ERROR);
				}
			}
			pid_list[i] = fork();
			if (pid_list[i] == 0)
			{
				if (temp_fd_in != STDIN_FILENO)
				{
					dup2(temp_fd_in, STDIN_FILENO);
					close (temp_fd_in);
				}
				if (temp_cmd->next)
				{
					close(pipe_fds[0]);
					dup2(pipe_fds[1], STDOUT_FILENO);
					close(pipe_fds[1]);
				}
				// if (temp_cmd->type == AST_CMD)
				// 	exit_value = f_redirect(temp_cmd->data.command.redirections);
				// else if (temp_cmd == AST_SUBSHELL)
				// 	exit_value = f_redirect(temp_cmd->data.subshell.redirections);
				// if (exit_value != SUCCESS)
				// 	exit(exit_value);
				exit(f_exec_cmd(temp_cmd));
			}
			else if (pid_list[i] > 0)
			{
				if (temp_fd_in != STDIN_FILENO)
					close(temp_fd_in);
				if (temp_cmd->next)
				{
					close(pipe_fds[1]);
					temp_fd_in = pipe_fds[0];
				}
				temp_cmd = temp_cmd->next;
				++i;
			}
			else
			{
				if (temp_cmd->next)
				{
					close(pipe_fds[0]);
					close(pipe_fds[1]);
				}
				f_cleanup_pipeline(pid_list, i, temp_fd_in);
				return (perror("fork"), ERROR);
			}
		}
		waitpid(pid_list[--i], &status, 0);
		if (WIFEXITED(status))
			exit_value = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			exit_value = 128 + WTERMSIG(status);
		while (--i >= 0)
			waitpid(pid_list[i], NULL, 0);
		free(pid_list);
		return (exit_value);
	}
}

int	f_exec_logic_expr(t_ast_node *expr)
{
	static int	exit_value = 0;

	if (!expr || expr->type != AST_LOGICAL_EXPR)
		return (ERROR);
	exit_value = f_exec_pipeline(expr->data.logical_expr.pipeline, exit_value); // TODO: pass exit_value down through all functions
	if ((expr->data.logical_expr.type == TYPE_AND
		&& exit_value == SUCCESS)
		|| (expr->data.logical_expr.type == TYPE_OR
		&& exit_value == FAIL))
		exit_value = f_exec_logic_expr(expr->next);
	return (exit_value);
}

void	f_execute_ast(t_ast_node *root)
{
	if (!root || root->type != AST_LOGICAL_EXPR)
		return (ERROR);
	f_exec_logic_expr(root);
	// TODO: save last exit value (maybe via f_setenv for "$?" or as a static int that is used if "$?" is called)
}
