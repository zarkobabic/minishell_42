/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_draft.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 22:13:45 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/08 19:04:44 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exe_execution.h"
#include <sys/wait.h>

int	f_exec_logic_expr(t_ast_node *expr, t_environ *env);

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

int	f_exec_builtin(t_cmd_ctx *ctx)
{
	if (ft_strncmp(ctx->argv[0], "echo", 5) == 0)
		return (f_echo(ctx->argc, ctx->argv));
	else if (ft_strncmp(ctx->argv[0], "cd", 3) == 0)
		return (f_cd(ctx->argc, ctx->argv));
	else if (ft_strncmp(ctx->argv[0], "pwd", 4) == 0)
		return (f_pwd());
	// else if (ft_strncmp(argv[0], "export", 7) == 0)
	// 	return (f_export(argc, argv));
	// else if (ft_strncmp(argv[0], "unset", 6) == 0)
	// 	return (f_unset(argc, argv));
	else if (ft_strncmp(ctx->argv[0], "env", 4) == 0)
		return (f_env(ctx->argc, ctx->argv));
	else if (ft_strncmp(ctx->argv[0], "exit", 5) == 0)
		return (f_exit(ctx->argc, ctx->argv, &ctx->error));
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

char **f_buildargv(t_ast_node *args, long *argc, t_environ *env) // FIXME: check when old_exit_value needs to be expanded and is maybe not needed anymore
{
	char	**argv;
	long	i;

	(void)env; //ADD VARIABLE EXPANSION THIS IS FOR $?
	if (!args || args->type != AST_IDENTIFIER)
		return (NULL);
	argv = (char **)malloc((*argc + 1) * sizeof(char *));
	if (!argv)
		return (NULL);
	i = 0;
	while (args && i < *argc)
	{
		argv[i] = args->data.identifier.string_value;
		args = args->next;
		++i;
	}
	argv[i] = NULL;
	if (args || i != *argc)
		return (free(argv), NULL);
	return (argv);
}

int	f_exec_cmd(t_ast_node *cmd, t_environ *env, int in_pipeline)
{
	if (!cmd || cmd->next != NULL)
		return (ERROR);
	if (cmd->type == AST_CMD)
		return (f_exec_simple_cmd(cmd, env, in_pipeline));
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

int	f_exec_logic_expr(t_ast_node *expr, t_environ *env)
{
	static int	exit_value = 0;

	if (!expr || expr->type != AST_LOGICAL_EXPR)
		return (ERROR);
	env->old_exit_value = exit_value;
	exit_value = f_exec_pipeline(expr->data.logical_expr.pipeline, env); // TODO: pass exit_value down through all functions
	env->old_exit_value = exit_value;
	if ((expr->data.logical_expr.type == TYPE_AND
		&& exit_value == SUCCESS)
		|| (expr->data.logical_expr.type == TYPE_OR
		&& exit_value != SUCCESS))
		exit_value = f_exec_logic_expr(expr->next, env);
	return (exit_value);
}

int	f_execute_ast(t_environ *env)
{
	if (!env->ast_root || env->ast_root->type != AST_LOGICAL_EXPR)
		return (ERROR);
	return (f_exec_logic_expr(env->ast_root, env));
}
