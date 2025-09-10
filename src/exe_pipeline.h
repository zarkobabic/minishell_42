/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_pipeline.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 17:16:17 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/10 02:20:31 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXE_PIPELINE_H
# define EXE_PIPELINE_H

# include <unistd.h>
# include "exe_execution.h"

int			f_exec_pipeline(t_ast_node *pipeline, t_env *env);

#endif