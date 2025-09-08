/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_pipeline.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 17:16:17 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/08 22:33:56 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXE_PIPELINE_H
# define EXE_PIPELINE_H

# include <unistd.h>
# include "exe_execution.h"

int			f_exec_pipeline(t_ast_node *pipeline, t_environ *env);

#endif