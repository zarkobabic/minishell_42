/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_pipeline.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 17:25:30 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/10 02:20:31 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exe_pipeline.h"
#include <sys/wait.h>

// Setup pipes for child process
static int	f_setup_child_pipes(t_pipeline_ctx *ctx, t_ast_node *temp_cmd)
{
	if (ctx->temp_fd_in != STDIN_FILENO)
	{
		if (dup2(ctx->temp_fd_in, STDIN_FILENO) < 0)
		{
			perror("dup2");
			if (temp_cmd->next)
			{
				close(ctx->pipe_fds[IN]);
				close(ctx->pipe_fds[OUT]);
			}
			close(ctx->temp_fd_in);
			return (ERROR);
		}
		close(ctx->temp_fd_in);
	}
	if (temp_cmd->next)
	{
		close(ctx->pipe_fds[IN]);
		if (dup2(ctx->pipe_fds[OUT], STDOUT_FILENO) < 0)
			return (perror("dup2"), close(ctx->pipe_fds[OUT]), ERROR);
		close(ctx->pipe_fds[OUT]);
	}
	return (SUCCESS);
}

// Execute child process
static void	f_execute_child_process(t_ast_node *temp_cmd, t_env *env)
{
	int	exit_value;

	exit_value = f_exec_cmd(temp_cmd, env, TRUE);
	f_cleanup_child(env->ast_root, NULL);
	mini_exit(exit_value, env);
}

// Handle parent process after fork
static void	f_handle_parent_process(t_pipeline_ctx *ctx, t_ast_node *temp_cmd)
{
	if (ctx->temp_fd_in != STDIN_FILENO)
		close(ctx->temp_fd_in);
	if (temp_cmd->next)
	{
		close(ctx->pipe_fds[OUT]);
		ctx->temp_fd_in = ctx->pipe_fds[IN];
	}
	ctx->current_index++;
}

// Handle fork error
static int	f_handle_fork_error(t_ast_node *temp_cmd, t_pipeline_ctx *ctx)
{
	if (temp_cmd->next)
	{
		close(ctx->pipe_fds[IN]);
		close(ctx->pipe_fds[OUT]);
	}
	f_cleanup_pipeline(ctx->pid_list, ctx->current_index, ctx->temp_fd_in);
	return (perror("fork"), ERROR);
}

// Handle child process
static int	f_handle_child_process(t_ast_node *temp_cmd, t_env *env,
		t_pipeline_ctx *ctx)
{
	if (f_setup_child_pipes(ctx, temp_cmd) == ERROR)
	{
		f_cleanup_child(env->ast_root, NULL);
		mini_exit(FAIL, env);
	}
	f_execute_child_process(temp_cmd, env);
	return (SUCCESS);
}

// Process single command
static int	f_process_single_cmd(t_ast_node *temp_cmd, t_env *env,
		t_pipeline_ctx *ctx)
{
	if (temp_cmd->next)
	{
		if (pipe(ctx->pipe_fds) < 0)
		{
			f_cleanup_pipeline(ctx->pid_list, ctx->current_index,
				ctx->temp_fd_in);
			return (perror("pipe"), ERROR);
		}
	}
	ctx->pid_list[ctx->current_index] = fork();
	if (ctx->pid_list[ctx->current_index] == 0)
		return (free(ctx->pid_list),
			f_handle_child_process(temp_cmd, env, ctx));
	else if (ctx->pid_list[ctx->current_index] > 0)
	{
		f_handle_parent_process(ctx, temp_cmd);
		return (SUCCESS);
	}
	else
		return (f_handle_fork_error(temp_cmd, ctx));
}

// Create all pipeline processes
static int	f_create_pipeline_processes(t_ast_node *pipeline, t_env *env,
		t_pipeline_ctx *ctx)
{
	t_ast_node	*temp_cmd;

	temp_cmd = pipeline->data.pipeline.cmds;
	while (temp_cmd)
	{
		if (f_process_single_cmd(temp_cmd, env, ctx) == ERROR)
			return (ERROR);
		temp_cmd = temp_cmd->next;
	}
	return (SUCCESS);
}

/// Wait for all pipeline processes to complete
static int	f_wait_for_pipeline(pid_t *pid_list, int cmd_count)
{
	int	exit_value;
	int	i;

	i = cmd_count - 1;
	exit_value = f_wait_for_child(pid_list[i]);
	while (--i >= 0)
		waitpid(pid_list[i], NULL, 0);
	return (exit_value);
}

int	f_exec_pipeline(t_ast_node *pipeline, t_env *env)
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
	error_value = f_create_pipeline_processes(pipeline, env, &ctx);
	if (error_value == ERROR)
		return (ERROR);
	exit_value = f_wait_for_pipeline(ctx.pid_list,
			pipeline->data.pipeline.cmd_cnt);
	free(ctx.pid_list);
	return (exit_value);
}
