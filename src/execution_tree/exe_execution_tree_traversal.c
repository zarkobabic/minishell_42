/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_execution_tree_traversal.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 09:35:20 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/16 16:07:07 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exe_execution.h"
#include "exe_redirection.h"
#include "exe_pipeline.h"
#include <sys/wait.h>

static int	f_exec_logic_expr(t_ast_node *expr, t_env *env);

static int	f_exec_subshell(t_ast_node *subshell, t_env *env)
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

static int	f_exec_pipeline(t_ast_node *pipeline, t_env *env)
{
	t_pipeline_ctx	ctx;
	int				exit_value;
	int				error_value;

	if (!pipeline || pipeline->type != AST_PIPELINE
		|| pipeline->data.pipeline.cmd_cnt <= 0
		|| !pipeline->data.pipeline.cmds)
		return (ERROR);
	if (pipeline->data.pipeline.cmd_cnt == 1)
		return (f_exec_cmd(pipeline->data.pipeline.cmds, env, FALSE));
	ctx.pid_list = (pid_t *)malloc(pipeline->data.pipeline.cmd_cnt
			* sizeof(pid_t));
	if (!ctx.pid_list)
		return (perror("malloc"), ERROR);
	ctx.current_index = 0;
	ctx.temp_fd_in = STDIN_FILENO;
	error_value = f_pipeline_create_processes(pipeline, env, &ctx);
	if (error_value == ERROR)
		return (ERROR);
	exit_value = f_wait_for_pipeline(ctx.pid_list,
			pipeline->data.pipeline.cmd_cnt);
	return (free(ctx.pid_list), exit_value);
}

static int	f_exec_logic_expr(t_ast_node *expr, t_env *env)
{
	if (!expr || expr->type != AST_LOGICAL_EXPR)
		return (ERROR);
	env->old_exit_value
		= f_exec_pipeline(expr->data.logical_expr.pipeline, env);
	if (g_received_signal != -1)
		env->old_exit_value = g_received_signal;
	if ((expr->data.logical_expr.type == TYPE_AND
			&& env->old_exit_value == SUCCESS)
		|| (expr->data.logical_expr.type == TYPE_OR
			&& env->old_exit_value != SUCCESS
			&& env->old_exit_value != CANCEL))
		env->old_exit_value = f_exec_logic_expr(expr->next, env);
	return (env->old_exit_value);
}

int	f_execute_ast(t_env *env)
{
	if (!env->ast_root || env->ast_root->type != AST_LOGICAL_EXPR)
		return (ERROR);
	if (g_received_signal != -1)
		env->old_exit_value = g_received_signal;
	g_received_signal = -1;
	ft_bzero(env->heredoclist, sizeof(env->heredoclist));
	if (f_heredoc_build_logical_expr(env->ast_root, env) != SUCCESS)
		return (GENERAL_ERR);
	if (g_received_signal != -1)
		return (env->old_exit_value = g_received_signal, env->old_exit_value);
	f_exec_logic_expr(env->ast_root, env);
	if (g_received_signal != -1)
		env->old_exit_value = g_received_signal;
	f_heredoc_delete_files(env);
	return (env->old_exit_value);
}
