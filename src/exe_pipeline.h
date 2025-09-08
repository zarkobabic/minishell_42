/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_pipeline.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 17:16:17 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/08 17:57:17 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXE_PIPELINE_H
# define EXE_PIPELINE_H

# include "exe_execution.h"
# include <unistd.h>

typedef struct s_pipeline_ctx
{
	pid_t	*pid_list;
	int		pipe_fds[2];
	int		current_index;
	int		temp_fd_in;
}			t_pipeline_ctx;

int			f_exec_pipeline(t_ast_node *pipeline, t_environ *env);

#endif