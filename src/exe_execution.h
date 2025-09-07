/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_execution.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 00:29:30 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/07 20:57:51 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "../libft/libft.h"
# include "../src/identifier.h"
# include "ast_tree_translator.h"
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <fcntl.h>
# include <stdbool.h>
# include "ast_tree_structures.h"

# define FALSE 0
# define TRUE 1
# define IN 0
# define OUT 1

enum	e_exit_value
{
	ERROR = -1,
	SUCCESS,
	FAIL
};

//builtin
int	f_pwd(void);
int	f_env(long argc, char **argv);
int	f_echo(long argc, char **argv);
int	f_cd(long argc, char **argv);
int	f_exit(long argc, char **argv);

//exe_draft
int	f_execute_ast(t_ast_node *root);

#endif