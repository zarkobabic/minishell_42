/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_pipeline_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 10:23:54 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/14 12:14:18 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exe_pipeline.h"

// Setup pipes for child process
int	f_pipeline_setup_child_pipes(t_pipeline_ctx *ctx, t_ast_node *temp_cmd)
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

// Wait for all pipeline processes to complete
int	f_wait_for_pipeline(pid_t *pid_list, int cmd_count)
{
	int	exit_value;
	int	i;

	i = cmd_count - 1;
	exit_value = f_wait_for_child(pid_list[i]);
	while (--i >= 0)
		waitpid(pid_list[i], NULL, 0);
	return (exit_value);
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

int	f_pipeline_handle_fork_error(t_ast_node *temp_cmd, t_pipeline_ctx *ctx)
{
	if (temp_cmd->next)
	{
		close(ctx->pipe_fds[IN]);
		close(ctx->pipe_fds[OUT]);
	}
	f_cleanup_pipeline(ctx->pid_list, ctx->current_index, ctx->temp_fd_in);
	return (perror("fork"), ERROR);
}
