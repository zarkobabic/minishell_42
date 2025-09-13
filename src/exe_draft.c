/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_draft.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 22:13:45 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/13 12:29:31 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exe_execution.h"
#include "exe_redirection.h"
#include <sys/wait.h>

int		f_exec_logic_expr(t_ast_node *expr, t_env *env);

bool	f_is_builtin_parent(char *call)
{
	return (!ft_strncmp(call, "cd", 3) || !ft_strncmp(call, "export", 7)
		|| !ft_strncmp(call, "unset", 6) || !ft_strncmp(call, "exit", 5));
}

bool	f_is_builtin_child(char *call)
{
	return (!ft_strncmp(call, "echo", 5) || !ft_strncmp(call, "pwd", 4)
		|| !ft_strncmp(call, "env", 4));
}

bool	f_is_builtin(char *call)
{
	return (f_is_builtin_parent(call) || f_is_builtin_child(call));
}

int	f_exec_builtin(t_cmd_ctx *ctx)
{
	if (ft_strncmp(ctx->argv[0], "echo", 5) == 0)
		return (f_echo(ctx->argc, ctx->argv));
	else if (ft_strncmp(ctx->argv[0], "cd", 3) == 0)
		return (f_cd(&ctx->env->minienviron, ctx->argc, ctx->argv));
	else if (ft_strncmp(ctx->argv[0], "pwd", 4) == 0)
		return (f_pwd());
	else if (ft_strncmp(ctx->argv[0], "export", 7) == 0)
		return (f_export(&ctx->env->minienviron, ctx->argc, ctx->argv));
	else if (ft_strncmp(ctx->argv[0], "unset", 6) == 0)
		return (f_unset(&ctx->env->minienviron, ctx->argc, ctx->argv));
	else if (ft_strncmp(ctx->argv[0], "env", 4) == 0)
		return (f_env(ctx->env->minienviron, ctx->argc, false));
	else if (ft_strncmp(ctx->argv[0], "exit", 5) == 0)
		return (f_exit(ctx->argc, ctx->argv, &ctx->error));
	else
		return (perror("builtin mapping"), ERROR);
}

void	f_cleanup_child(t_ast_node *root, char **argv)
{
	ast_free_tree(root);
	if (argv)
		free_argv(&argv);
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

int	f_exec_subshell(t_ast_node *subshell, t_env *env)
{
	pid_t	pid;
	int		exit_value;

	pid = fork();
	if (pid == 0)
	{
		exit_value = SUCCESS;
		exit_value = f_redirect(subshell->data.subshell.redirections, env);
		if (exit_value == SUCCESS)
			exit_value = f_exec_logic_expr(subshell->data.subshell.logical_expr,
					env);
		ast_free_tree(env->ast_root);
		mini_exit(exit_value, env);
	}
	else if (pid > 0)
		exit_value = f_wait_for_child(pid);
	else
		return (perror("fork"), ast_free_tree(env->ast_root), ERROR);
	return (exit_value);
}

int	f_exec_cmd(t_ast_node *cmd, t_env *env, bool in_pipeline)
{
	if (!cmd)
		return (ERROR);
	if (cmd->type == AST_CMD)
		return (f_exec_simple_cmd(cmd, env, in_pipeline));
	else if (cmd->type == AST_SUBSHELL)
		return (f_exec_subshell(cmd, env));
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

int	f_exec_logic_expr(t_ast_node *expr, t_env *env)
{
	extern int	g_received_signal;

	if (!expr || expr->type != AST_LOGICAL_EXPR)
		return (ERROR);
	env->old_exit_value = f_exec_pipeline(expr->data.logical_expr.pipeline, env);
	if (g_received_signal != -1)
		env->old_exit_value = g_received_signal;
	if ((expr->data.logical_expr.type == TYPE_AND && env->old_exit_value == SUCCESS)
		|| (expr->data.logical_expr.type == TYPE_OR && (env->old_exit_value == FAIL || env->old_exit_value == ERROR)))
		env->old_exit_value = f_exec_logic_expr(expr->next, env);
	return (env->old_exit_value);
}

int	f_execute_ast(t_env *env)
{
	extern int g_received_signal;

	if (!env->ast_root || env->ast_root->type != AST_LOGICAL_EXPR)
		return (ERROR);
	if (g_received_signal != -1)
		env->old_exit_value = g_received_signal;
	g_received_signal = -1;
	ft_bzero(env->heredoclist, sizeof(env->heredoclist));
	if (f_heredoc_build_logical_expr(env->ast_root, env) != SUCCESS)
		return (GENERAL);
	if (g_received_signal != -1)
		return (env->old_exit_value = g_received_signal, env->old_exit_value);
	f_exec_logic_expr(env->ast_root, env);
	if (g_received_signal != -1)
		env->old_exit_value = g_received_signal;
	f_heredoc_delete_files(env);
	return (env->old_exit_value);
}
