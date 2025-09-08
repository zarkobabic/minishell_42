/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_simple_command.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 19:04:18 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/09 01:24:05 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exe_execution.h"

// Prepare argv for command execution
static int	f_prepare_argv(t_ast_node *simcmd, t_cmd_ctx *ctx)
{
	ctx->argc = simcmd->data.command.arg_cnt;
	ctx->argv = f_buildargv(simcmd->data.command.args, &ctx->argc, ctx->env);
	if (!ctx->argv)
		return (perror("arg list"), ERROR);
	return (SUCCESS);
}

// Setup file descriptors for builtin parent execution
static int	f_setup_builtin_parent_fds(t_cmd_ctx *ctx)
{
	ctx->orig_fds[IN] = dup(STDIN_FILENO);
	ctx->orig_fds[OUT] = dup(STDOUT_FILENO);
	if (ctx->orig_fds[IN] == -1 || ctx->orig_fds[OUT] == -1)
	{
		if (ctx->orig_fds[IN] != -1)
			close(ctx->orig_fds[IN]);
		if (ctx->orig_fds[OUT] != -1)
			close(ctx->orig_fds[OUT]);
		free(ctx->argv);
		return (perror("dup"), ERROR);
	}
	return (SUCCESS);
}

// Execute builtin command in parent process
static int	f_exec_builtin_parent(t_cmd_ctx *ctx)
{
	int	exit_value;

	if (f_setup_builtin_parent_fds(ctx) == ERROR)
		return (ERROR);
	exit_value = SUCCESS;
	// exit_value = f_redirect(ctx->redirections);
	if (exit_value == SUCCESS)
		exit_value = f_exec_builtin(ctx);
	if (dup2(ctx->orig_fds[IN], STDIN_FILENO) == -1)
		perror("dup2 restore in");
	if (dup2(ctx->orig_fds[OUT], STDOUT_FILENO) == -1)
		perror("dup2 restore out");
	close(ctx->orig_fds[IN]);
	close(ctx->orig_fds[OUT]);
	if (ft_strncmp(ctx->argv[0], "exit", 5) == 0 && ctx->error == false)
	{
		free(ctx->argv);
		ast_free_tree(ctx->env->ast_root);
		mini_exit(exit_value, ctx->env);
	}
	return (exit_value);
}

// Handle child process execution
static int	f_handle_child_process(t_cmd_ctx *ctx)
{
	int	exit_value;

	exit_value = SUCCESS;
	// exit_value = f_redirect(ctx->redirections);
	if (exit_value == SUCCESS)
	{
		if (f_is_builtin_child(ctx->argv[0]))
			exit_value = f_exec_builtin(ctx);
		//Add other executables here after
	}
	f_cleanup_child(ctx->env->ast_root, ctx->argv);
	mini_exit(exit_value, ctx->env);
	return (FAIL);
}

// Execute external command
static int	f_exec_external_cmd(t_cmd_ctx *ctx)
{
	pid_t	pid;
	int		exit_value;

	pid = fork();
	if (pid == 0)
		return (f_handle_child_process(ctx));
	else if (pid > 0)
		exit_value = f_wait_for_child(pid);
	else
	{
		free(ctx->argv);
		return (perror("fork"), ERROR);
	}
	return (exit_value);
}

// Execute command in pipeline
static int	f_exec_in_pipeline(t_cmd_ctx *ctx)
{
	int	exit_value;

	exit_value = SUCCESS;
	// exit_value = f_redirect(ctx->redirections);
	if (exit_value != SUCCESS)
		return (exit_value);
	if (f_is_builtin(ctx->argv[0]))
		exit_value = f_exec_builtin(ctx);
	return (exit_value);
}

// Main simple command execution function
int	f_exec_simple_cmd(t_ast_node *simcmd, t_environ *env, int in_pipeline)
{
	t_cmd_ctx	ctx;
	int			exit_value;

	if (simcmd->data.command.arg_cnt <= 0 || !simcmd->data.command.args)
		return (SUCCESS);
	ctx.env = env;
	ctx.error = false;
	ctx.redirections = simcmd->data.command.redirections;
	ctx.in_pipeline = in_pipeline;
	if (f_prepare_argv(simcmd, &ctx) == ERROR)
		return (ERROR);
	if (!in_pipeline && f_is_builtin_parent(ctx.argv[0]))
		exit_value = f_exec_builtin_parent(&ctx);
	else if (!in_pipeline)
		exit_value = f_exec_external_cmd(&ctx);
	else
		exit_value = f_exec_in_pipeline(&ctx);
	free(ctx.argv);
	return (exit_value);
}
