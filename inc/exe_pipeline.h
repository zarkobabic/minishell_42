/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_pipeline.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 17:16:17 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/14 12:14:32 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXE_PIPELINE_H
# define EXE_PIPELINE_H

# include "exe_execution.h"

# define IN 0
# define OUT 1

typedef struct s_pipeline_ctx
{
	pid_t		*pid_list;
	int			pipe_fds[2];
	int			current_index;
	int			temp_fd_in;
}				t_pipeline_ctx;

void	f_cleanup_pipeline(pid_t *pid_list, int created_processes, int temp_fd);
int		f_pipeline_create_processes(t_ast_node *pipeline, t_env *env,
			t_pipeline_ctx *ctx);
int		f_pipeline_handle_fork_error(t_ast_node *temp_cmd, t_pipeline_ctx *ctx);
int		f_pipeline_setup_child_pipes(t_pipeline_ctx *ctx, t_ast_node *temp_cmd);
int		f_wait_for_pipeline(pid_t *pid_list, int cmd_count);

#endif
