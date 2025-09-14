/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_simple_cmd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 19:04:18 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/14 11:24:48 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exe_execution.h"
#include "exe_redirection.h"

// Handle child process execution
static int	f_simple_cmd_handle_child(t_cmd_ctx *ctx)
{
	int	exit_value;

	exit_value = SUCCESS;
	exit_value = f_redirect(ctx->redirections, ctx->env);
	if (exit_value == SUCCESS)
	{
		if (f_is_builtin_child(ctx->argv[0]))
			exit_value = f_exec_builtin(ctx);
		else
			exit_value = f_exec_external(ctx);
	}
	f_cleanup_child(ctx->env->ast_root, ctx->argv);
	mini_exit(exit_value, ctx->env);
	return (exit_value);
}

// Execute external command
static int	f_simple_cmd_fork(t_cmd_ctx *ctx)
{
	pid_t	pid;
	int		exit_value;

	pid = fork();
	if (pid == 0)
		return (f_simple_cmd_handle_child(ctx));
	else if (pid > 0)
		exit_value = f_wait_for_child(pid);
	else
	{
		free_argv(&ctx->argv);
		return (perror("fork"), ERROR);
	}
	return (exit_value);
}

// Execute command in pipeline
static int	f_simple_cmd_in_pipeline(t_cmd_ctx *ctx)
{
	int	exit_value;

	exit_value = f_redirect(ctx->redirections, ctx->env);
	if (exit_value != SUCCESS)
		return (exit_value);
	if (f_is_builtin(ctx->argv[0]))
		exit_value = f_exec_builtin(ctx);
	else
		exit_value = f_exec_external(ctx);
	return (exit_value);
}

int	f_exec_simple_cmd(t_ast_node *simcmd, t_env *env, int in_pipeline)
{
	t_cmd_ctx	ctx;
	int			exit_value;

	ctx.env = env;
	ctx.error = false;
	ctx.redirections = simcmd->data.command.redirections;
	ctx.in_pipeline = in_pipeline;
	if (f_prepare_argv(simcmd, &ctx) == ERROR)
		return (ERROR);
	if (!ctx.argv)
		f_create_empty_argv(&ctx.argv);
	if (!in_pipeline && f_is_builtin_parent(ctx.argv[0]))
		exit_value = f_exec_builtin_parent(&ctx);
	else if (!in_pipeline)
		exit_value = f_simple_cmd_fork(&ctx);
	else
		exit_value = f_simple_cmd_in_pipeline(&ctx);
	free_argv(&ctx.argv);
	return (exit_value);
}
