/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draft.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 22:13:45 by eberkau           #+#    #+#             */
/*   Updated: 2025/08/30 18:59:05 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include "../src/ast_tree_structures.h"

int	f_is_builtin(char *name)
{
	return (!ft_strcmp(name, "echo")
		|| !ft_strcmp(name, "cd")
		|| !ft_strcmp(name, "pwd")
		|| !ft_strcmp(name, "export")
		|| !ft_strcmp(name, "unset")
		|| !ft_strcmp(name, "env")
		|| !ft_strcmp(name, "exit"));
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
		return (f_cd(argv, argc));
	else if (ft_strncmp(argv[0], "cd", 9) == 0)
		return (f_echo(argv, argc));
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

	pid = fork();
	if (pid == 0)
	{
		// TODO: apply redirections
		f_exec_logic_expr(subshell->data.subshell.logical_expr);
	}
	else
		waitpid;
	return ();
}

int	f_exec_cmd(t_ast_node *cmd)
{
	if (cmd->next != NULL)
		return (ERROR);
	if (cmd->type == AST_CMD)
		return (f_exec_simple_cmd(cmd));
	else if (cmd->type == AST_SUBSHELL)
		return (f_exec_subshell(cmd));
	else
		return (ERROR);
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
		int		pipe_fds[2];
		int		temp_in_fd;
		pid_t	pid;
		pid_t	*pid_list;
		t_ast_node	*temp_cmd;

		temp_cmd = pipeline->data.pipeline.cmds;
		while (temp_cmd)
		{
			if (temp_cmd->next)
			{
				if (pipe(pipe_fds) < 0)
					return (perror("pipe"), ERROR);
			}
			pid = fork();
			if (pid == 0)
			{
				if (temp_in_fd != STDIN_FILENO)
				{
					dup2(temp_in_fd, STDIN_FILENO);
					close (temp_in_fd);
				}
				if (temp_cmd->next)
				{
					close(pipe_fds[0]);
					dup2(pipe_fds[1], STDOUT_FILENO);
					close(pipe_fds[1]);
				}
				// f_exec_cmd(temp_cmd);
				exit(f_exec_cmd(temp_cmd)); // FIXME: revise child exit logic
			}
			else if (pid > 0)
			{
				if (temp_in_fd != STDIN_FILENO)
					close(temp_in_fd);
				if (temp_cmd->next)
				{
					close(pipe_fds[1]);
					temp_in_fd = pipe_fds[0];
				}
				temp_cmd = temp_cmd->next;
				++i;
			} // TODO: catch exit status of last pipeline element
			else
				return (perror("fork"), ERROR); // BUG: potentially leaks open fds
		}
		while (i-- > 0)
			wait(NULL); // FIXME: probably waitpid to catch exit statuses, and store them in an array or so


		return (f_pipeline_logic(/* see big code block above */));
	}
}

int	f_exec_logic_expr(t_ast_node *expr)
{
	int	exit_value;

	if (!expr || expr->type != AST_LOGICAL_EXPR)
		return (ERROR);
	exit_value = f_exec_pipeline(expr->data.logical_expr.pipeline);
	if ((expr->data.logical_expr.type == TYPE_AND
		&& exit_value == SUCCESS)
		|| (expr->data.logical_expr.type == TYPE_OR
		&& exit_value == FAIL))
		exit_value = f_exec_logic_expr(expr->next);
	return (exit_value);
}

int	f_execute_ast(t_ast_node *root)
{
	int	exit_value;

	if (!root || root->type != AST_LOGICAL_EXPR)
		return (ERROR);
	exit_value = f_exec_logic_expr(root);
	// TODO: save last exit value (maybe via f_setenv for "$?" or as a static int that is used if "$?" is called)
	return (exit_value);
}
