/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_redirection.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 02:31:55 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/14 12:15:26 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXE_REDIRECTION_H
# define EXE_REDIRECTION_H

# include "exe_environment.h"
# include "exe_execution.h"
# include <stdio.h>

// redirection

int		f_redirect(t_ast_node *redirections, t_env *env);

// utils

char	**f_expand_vars(t_ast_node *redirection, t_env *env);
char	*f_heredoc_find_entry(t_ast_node *target, t_env *env);

// heredoc management

int		f_heredoc_build_logical_expr(t_ast_node *ast_logical_expr, t_env *env);
void	f_heredoc_delete_files(t_env *env);
void	f_heredoc_populate_entry_file_name(t_env *env, int index);
int		f_heredoc_read_input(t_env *env, int i, char *delimiter);

#endif
