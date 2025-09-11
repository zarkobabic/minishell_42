/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_pipeline.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 17:16:17 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/11 02:33:03 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXE_PIPELINE_H
# define EXE_PIPELINE_H

# include "exe_execution.h"

# define IN 0
# define OUT 1

int			f_exec_pipeline(t_ast_node *pipeline, t_env *env);

#endif