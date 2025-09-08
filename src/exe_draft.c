/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_draft.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 22:13:45 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/08 02:16:34 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exe_execution.h"
#include <sys/wait.h>

int	f_exec_logic_expr(t_ast_node *expr, t_ast_node *root);

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

int	f_exec_builtin(long argc, char **argv)
{
	if (ft_strncmp(argv[0], "echo", 5) == 0)
		return (f_echo(argc, argv));
	else if (ft_strncmp(argv[0], "cd", 3) == 0)
		return (f_cd(argc, argv));
	else if (ft_strncmp(argv[0], "pwd", 4) == 0)
		return (f_pwd());
	// else if (ft_strncmp(argv[0], "export", 7) == 0)
	// 	return (f_export(argc, argv));
	// else if (ft_strncmp(argv[0], "unset", 6) == 0)
	// 	return (f_unset(argc, argv));
	else if (ft_strncmp(argv[0], "env", 4) == 0)
		return (f_env(argc, argv));
	else if (ft_strncmp(argv[0], "exit", 5) == 0)
		return (f_exit(argc, argv));
	else
		return (perror("builtin mapping"), ERROR);
}

void	f_cleanup_child(t_ast_node *root, char **argv)
{
	ast_free_tree(root);
	if (argv)
		free(argv);
}

int	f_wait_for_child(pid_t pid)
{
	int	status;
	int	exit_value;
	
	if (waitpid(pid, &status, 0) < 0)
		return (ERROR);
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
	argv = (char **)malloc((argc + 1) * sizeof(char *));
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

int f_exec_simple_cmd(t_ast_node *simcmd, t_ast_node *root, int old_exit_value, int in_pipeline)
{
	char	**argv;
	long	argc;
	pid_t	pid;
	int		exit_value;
	int		orig_fds[2];
	
	if (simcmd->data.command.arg_cnt <= 0 || !simcmd->data.command.args)
		return (SUCCESS);
	// TODO: variable expansion
	argc = simcmd->data.command.arg_cnt;
	argv = f_buildargv(simcmd->data.command.args, argc, old_exit_value); // TODO: var expansion
	if (!argv)
		return (perror("arg list"), ERROR);
	if (!in_pipeline && f_is_builtin_parent(argv[0]))
	{
		orig_fds[IN] = dup(STDIN_FILENO);
		orig_fds[OUT] = dup(STDOUT_FILENO);
		if (orig_fds[IN] == -1 || orig_fds[OUT] == -1)
		{
			if (orig_fds[IN] != -1)
				close(orig_fds[IN]);
			if (orig_fds[OUT] != -1)
				close(orig_fds[OUT]);
			free(argv);
			return (perror("dup"), ERROR);
		}
		// exit_value = f_redirect(simcmd->data.command.redirections);
		exit_value = SUCCESS;
		if (exit_value == SUCCESS)
			exit_value = f_exec_builtin(argc, argv);
		if (dup2(orig_fds[IN], STDIN_FILENO) == -1)
			perror("dup2 restore in");
		if (dup2(orig_fds[OUT], STDOUT_FILENO) == -1)
			perror("dup2 restore out");
		close(orig_fds[IN]);
		close(orig_fds[OUT]);
		if(ft_strncmp(argv[0], "exit", 5) == 0)
		{
			free(argv);
			ast_free_tree(root);
			exit(exit_value);
		}
	}
	else if (!in_pipeline)
	{
		pid = fork();
		if (pid == 0)
		{
			// exit_value = f_redirect(simcmd->data.command.redirections);
			exit_value = SUCCESS;
			if (exit_value != SUCCESS)
			{
				f_cleanup_child(root, argv);
				exit(exit_value);
			}
			if (f_is_builtin_child(argv[0]))
				exit_value = f_exec_builtin(argc, argv);
			// else
			// 	exit_value = f_exec_file(argc, argv);
			f_cleanup_child(root, argv);
			exit(exit_value);
		}
		else if (pid > 0)
			exit_value = f_wait_for_child(pid);
		else
		{
			free(argv);
			return (perror("fork"), ERROR);
		}
	}
	else
	{
		// exit_value = f_redirect(simcmd->data.command.redirections);
		exit_value = SUCCESS;
		if (exit_value != SUCCESS)
		{
			free(argv); //proveri
			return (exit_value);
		}
		if (f_is_builtin(argv[0]))
			exit_value = f_exec_builtin(argc, argv);
		// else
		// 	exit_value = f_exec_file(argc, argv);
	}
	free(argv); // TODO: deep free everything, not just shallow
	return (exit_value);
}

int	f_exec_cmd(t_ast_node *cmd, t_ast_node *root, int old_exit_value, int in_pipeline)
{
	if (!cmd || cmd->next != NULL)
		return (ERROR);
	if (cmd->type == AST_CMD)
		return (f_exec_simple_cmd(cmd, root, old_exit_value, in_pipeline));
	// else if (cmd->type == AST_SUBSHELL)
	// 	return (f_exec_subshell(cmd, old_exit_value, in_pipeline));
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

int	f_exec_pipeline(t_ast_node *pipeline, t_ast_node *root, int old_exit_value)
{
	if (!pipeline || pipeline->type != AST_PIPELINE
		|| pipeline->data.pipeline.cmd_cnt <= 0 || !pipeline->data.pipeline.cmds)
		return (ERROR);
	if (pipeline->data.pipeline.cmd_cnt == 1)
		return (f_exec_cmd(pipeline->data.pipeline.cmds, root, old_exit_value, FALSE));
	else
	{
		int		i;
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
					if (dup2(temp_fd_in, STDIN_FILENO) < 0)
					{
						perror("dup2");
						if (temp_cmd->next)
						{
							close(pipe_fds[IN]);
							close(pipe_fds[OUT]);
						}
						close(temp_fd_in);
						f_cleanup_child(root, NULL);
						free(pid_list);
						exit(FAIL);
					}
					close (temp_fd_in);
				}
				if (temp_cmd->next)
				{
					close(pipe_fds[IN]); //While refactoring keep the order of this close and if
					if (dup2(pipe_fds[OUT], STDOUT_FILENO) < 0)
					{
						perror("dup2");
						close(pipe_fds[OUT]);
						f_cleanup_child(root, NULL);
						free(pid_list);
						exit(FAIL);
					}
					close(pipe_fds[OUT]);
				}
				exit_value = f_exec_cmd(temp_cmd, root, old_exit_value, TRUE);
				f_cleanup_child(root, NULL);
				free(pid_list);
				exit(exit_value);
			}
			else if (pid_list[i] > 0)
			{
				if (temp_fd_in != STDIN_FILENO)
					close(temp_fd_in);
				if (temp_cmd->next)
				{
					close(pipe_fds[OUT]);
					temp_fd_in = pipe_fds[IN];
				}
				temp_cmd = temp_cmd->next;
				++i;
			}
			else
			{
				if (temp_cmd->next)
				{
					close(pipe_fds[IN]);
					close(pipe_fds[OUT]);
				}
				f_cleanup_pipeline(pid_list, i, temp_fd_in);
				return (perror("fork"), ERROR);
			}
		}
		exit_value = f_wait_for_child(pid_list[--i]);
		while (--i >= 0)
			waitpid(pid_list[i], NULL, 0);
		free(pid_list);
		return (exit_value);
	}
}

int	f_exec_logic_expr(t_ast_node *expr, t_ast_node *root)
{
	static int	exit_value = 0;

	if (!expr || expr->type != AST_LOGICAL_EXPR)
		return (ERROR);
	exit_value = f_exec_pipeline(expr->data.logical_expr.pipeline, root, exit_value); // TODO: pass exit_value down through all functions
	if ((expr->data.logical_expr.type == TYPE_AND
		&& exit_value == SUCCESS)
		|| (expr->data.logical_expr.type == TYPE_OR
		&& exit_value != SUCCESS))
		exit_value = f_exec_logic_expr(expr->next, root);
	return (exit_value);
}

int	f_execute_ast(t_ast_node *root)
{
	if (!root || root->type != AST_LOGICAL_EXPR)
		return (ERROR);
	return (f_exec_logic_expr(root, root)); // TODO: save last exit value (maybe via f_setenv for "$?" or as a static int that is used if "$?" is called)
}
