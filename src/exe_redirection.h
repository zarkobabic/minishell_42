/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_redirection.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 02:31:55 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/12 09:10:01 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXE_REDIRECTION_H
# define EXE_REDIRECTION_H

# include "exe_execution.h"
# include <readline/readline.h>

# define HEREDOC_FILE_NAME "/tmp/.deEZ_guys_minishell_tempheredocfile.txt"

int	f_redirect(t_ast_node *redirections, t_env *env, char **argv);

#endif