/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_redirection.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 02:31:55 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/13 12:27:13 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXE_REDIRECTION_H
# define EXE_REDIRECTION_H

# include "exe_environment.h"
# include "exe_execution.h"
# include <stdio.h>
# include <readline/readline.h>

int	f_redirect(t_ast_node *redirections, t_env *env);

int		f_heredoc_build_logical_expr(t_ast_node *ast_logical_expr, t_env *env);
int		f_heredoc_setup_files(t_ast_node *cmd, t_env *env);
void	f_heredoc_delete_files(t_env *env);

#endif