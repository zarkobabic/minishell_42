/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_pipeline.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 17:25:30 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/14 10:54:19 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exe_pipeline.h"
#include <sys/wait.h>

static void	f_pipeline_execute_child(t_ast_node *temp_cmd, t_env *env)
{
	int	exit_value;

	exit_value = f_exec_cmd(temp_cmd, env, TRUE);
	f_cleanup_child(env->ast_root, NULL);
	mini_exit(exit_value, env);
}

static int	f_pipeline_handle_child(t_ast_node *temp_cmd, t_env *env,
	t_pipeline_ctx *ctx)
{
	if (f_pipeline_setup_child_pipes(ctx, temp_cmd) == ERROR)
	{
		f_cleanup_child(env->ast_root, NULL);
		mini_exit(GENERAL_ERR, env);
	}
	f_pipeline_execute_child(temp_cmd, env);
	return (SUCCESS);
}

// Handle parent process after fork
static void	f_pipeline_handle_parent(t_pipeline_ctx *ctx, t_ast_node *temp_cmd)
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

// Process single command
static int	f_pipeline_process_cmd(t_ast_node *temp_cmd, t_env *env,
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
			f_pipeline_handle_child(temp_cmd, env, ctx));
	else if (ctx->pid_list[ctx->current_index] > 0)
	{
		f_pipeline_handle_parent(ctx, temp_cmd);
		return (SUCCESS);
	}
	else
		return (f_pipeline_handle_fork_error(temp_cmd, ctx));
}

int	f_pipeline_create_processes(t_ast_node *pipeline, t_env *env,
		t_pipeline_ctx *ctx)
{
	t_ast_node	*temp_cmd;

	temp_cmd = pipeline->data.pipeline.cmds;
	while (temp_cmd)
	{
		if (f_pipeline_process_cmd(temp_cmd, env, ctx) == ERROR)
			return (ERROR);
		temp_cmd = temp_cmd->next;
	}
	return (SUCCESS);
}
