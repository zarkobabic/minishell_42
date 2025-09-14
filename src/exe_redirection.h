/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_redirection.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 02:31:55 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/14 09:14:01 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXE_REDIRECTION_H
# define EXE_REDIRECTION_H

# include "exe_environment.h"
# include "exe_execution.h"
# include <readline/readline.h>
# include <stdio.h>

int		f_redirect(t_ast_node *redirections, t_env *env);

int		f_heredoc_build_logical_expr(t_ast_node *ast_logical_expr, t_env *env);
int		f_heredoc_setup_files(t_ast_node *cmd, t_env *env);
void	f_heredoc_delete_files(t_env *env);

#endif