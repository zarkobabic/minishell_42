/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_draft.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 22:13:45 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/06 20:30:07 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exe_execution.h"
#include "../src/ast_tree_structures.h"

#include "exe_builtins.c"
#include "exe_redirection.c"
int	f_exec_logic_expr(t_ast_node *expr);


bool	f_is_builtin_parent(char *call)
{
	return (!ft_strncmp(call, "cd", 3)
		|| !ft_strncmp(call, "export", 7)
		|| !ft_strncmp(call, "unset", 6)
		|| !ft_strncmp(call, "exit", 5));
}

bool	f_is_builtin_child(char *call)
{
	return (!ft_strncmp(call, "echo", 5)
		|| !ft_strncmp(call, "pwd", 4)
		|| !ft_strncmp(call, "env", 4));
}

bool	f_is_builtin(char *call)
{
	return (f_is_builtin_parent(call)
		|| f_is_builtin_child(call));
}

// int	f_execute_cmd(t_ast_node *cmd)
// {
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
// }

int	f_exec_builtin(long argc, char **argv)
{
	if (ft_strncmp(argv[0], "echo", 5) == 0)
		return (f_echo(argv, argc));
	else if (ft_strncmp(argv[0], "cd", 3) == 0)
		return (f_cd(argv, argc));
	else if (ft_strncmp(argv[0], "pwd", 4) == 0)
		return (f_pwd());
	// else if (ft_strncmp(argv[0], "export", 7) == 0)
	// 	return (f_export(argv, argc));
	// else if (ft_strncmp(argv[0], "unset", 6) == 0)
	// 	return (f_unset(argv, argc));
	else if (ft_strncmp(argv[0], "env", 4) == 0)
		return (f_env(argv, argc));
	else if (ft_strncmp(argv[0], "exit", 5) == 0)
		return (f_exit(argv, argc));
	else
		return (perror("builtin mapping"), ERROR);
}

int	f_wait_for_child(pid_t pid)
{
	int	status;
	int	exit_value;
	
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		exit_value = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		exit_value = 128 + WTERMSIG(status);
	else
		exit_value = ERROR;
	return (exit_value);
}

char **f_buildargv(t_ast_node *args, long argc, int old_exit_value) // FIXME: check when old_exit_value needs to be expanded and is maybe not needed anymore
{
	char	**argv;
	long	i;

	(void)old_exit_value;
	if (!args || args->type != AST_IDENTIFIER)
		return (NULL);
	argv = (char **)malloc((argc + 1) * sizeof(char *)); // include NULL slot
	if (!argv)
		return (NULL);
	i = 0;
	while (args && i < argc)
	{
		argv[i] = args->data.identifier.string_value;
		args = args->next;
		++i;
	}
	argv[i] = NULL;
	if (args || i != argc)
		return (free(argv), NULL);
	return (argv);
}

int f_exec_simple_cmd(t_ast_node *simcmd, int old_exit_value, int in_pipeline)
{
	char	**argv;
	long	argc;
	pid_t	pid;
	int		exit_value;
	int		orig_in;
	int		orig_out;
	
	argc = simcmd->data.command.arg_cnt;
	argv = f_buildargv(simcmd->data.command.args, argc, old_exit_value); // TODO: var expansion
	if (!argv)
		return (perror("arg list"), ERROR);
	// Case 1: normal (not in pipeline child) parent-builtin executes in current shell
	if (!in_pipeline && argv[0] && f_is_builtin_parent(argv[0]))
	{
		orig_in = dup(STDIN_FILENO);
		orig_out = dup(STDOUT_FILENO);
		if (orig_in == -1 || orig_out == -1)
		{
			if (orig_in != -1)
				close(orig_in);
			if (orig_out != -1)
				close(orig_out);
			free(argv);
			return (perror("dup"), ERROR);
		}
		exit_value = f_redirect(simcmd->data.command.redirections);
		if (exit_value == SUCCESS)
			exit_value = f_exec_builtin(argv, argc); // execute without fork
		// restore FDs regardless
		if (dup2(orig_in, STDIN_FILENO) == -1)
			perror("dup2 restore in");
		if (dup2(orig_out, STDOUT_FILENO) == -1)
			perror("dup2 restore out");
		close(orig_in);
		close(orig_out);
	}
	else if (!in_pipeline)
	{
		pid = fork();
		if (pid == 0)
		{
			exit_value = f_redirect(simcmd->data.command.redirections);
			if (exit_value != SUCCESS)
				exit(exit_value);
			if (argv[0] && f_is_builtin_child(argv[0]))
				exit_value = f_exec_builtin(argv, argc);
			// else if (argv[0])
			// 	exit_value = f_exec_file(argv, argc);
			else
				exit_value = FAIL;
			exit(exit_value);
		}
		else if (pid > 0)
		{
			// TODO: outsource following logic to separate function due to repetitional appearance
			// waitpid(pid, &status, 0);
			// if (WIFEXITED(status))
			// 	exit_value = WEXITSTATUS(status);
			// else if (WIFSIGNALED(status))
			// 	exit_value = 128 + WTERMSIG(status);
			exit_value = f_wait_for_child(pid);
		}
		else
			return (perror("fork"), ERROR);
	}
	else
	{
		exit_value = f_redirect(simcmd->data.command.redirections);
		if (exit_value != SUCCESS)
			return (exit_value);
		if (argv[0] && f_is_builtin(argv[0]))
			exit_value = f_exec_builtin(argv, argc);
		// else if (argv[0])
		// 	exit_value = f_exec_file(argv, argc);
		else
			exit_value = FAIL; // empty command: no-op
	}
	free(argv); // TODO: check, if viable at this point
	return (exit_value);
}

int	f_exec_subshell(t_ast_node *subshell, int old_exit_value, int in_pipeline)
{
	pid_t	pid;
	int		exit_value;

	(void)old_exit_value;
	if (in_pipeline)
	{
		exit_value = f_redirect(subshell->data.subshell.redirections);
		if (exit_value != SUCCESS)
			return(exit_value);
		exit_value = f_exec_logic_expr(subshell->data.subshell.logical_expr);
	}
	else
	{
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
			// waitpid(pid, &status, 0);
			// if (WIFEXITED(status))
			// 	exit_value = WEXITSTATUS(status);
			// else if (WIFSIGNALED(status))
			// 	exit_value = 128 + WTERMSIG(status);
			exit_value = f_wait_for_child(pid);
		}
		else
			return (perror("fork"), ERROR);
	}
	return (exit_value);
}

int	f_exec_cmd(t_ast_node *cmd, int old_exit_value, int in_pipeline)
{
	if (cmd->next != NULL)
		return (ERROR);
	// TODO: variable expansion
	if (cmd->type == AST_CMD)
		return (f_exec_simple_cmd(cmd, old_exit_value, in_pipeline));
	else if (cmd->type == AST_SUBSHELL)
		return (f_exec_subshell(cmd, old_exit_value, in_pipeline));
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

int	f_exec_pipeline(t_ast_node *pipeline, int old_exit_value)
{
	if (!pipeline || pipeline->type != AST_PIPELINE
		|| pipeline->data.pipeline.cmd_cnt <= 0 || !pipeline->data.pipeline.cmds)
		return (ERROR);
	if (pipeline->data.pipeline.cmd_cnt == 1)
		return (f_exec_cmd(pipeline->data.pipeline.cmds, old_exit_value, FALSE));
	else
	{
		int		i;
		// int		status;
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
				exit(f_exec_cmd(temp_cmd, old_exit_value, TRUE));
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
		// waitpid(pid_list[--i], &status, 0);
		// if (WIFEXITED(status))
		// 	exit_value = WEXITSTATUS(status);
		// else if (WIFSIGNALED(status))
		// 	exit_value = 128 + WTERMSIG(status);
		exit_value = f_wait_for_child(pid_list[--i]);
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

int	f_execute_ast(t_ast_node *root)
{
	if (!root || root->type != AST_LOGICAL_EXPR)
		return (ERROR);
	return (f_exec_logic_expr(root));
	// TODO: save last exit value (maybe via f_setenv for "$?" or as a static int that is used if "$?" is called)
}
