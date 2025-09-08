/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_execution.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 00:29:30 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/08 17:51:54 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "../libft/libft.h"
# include "../src/identifier.h"
# include "ast_tree_structures.h"
# include "ast_tree_translator.h"
# include <fcntl.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

# define FALSE 0
# define TRUE 1
# define IN 0
# define OUT 1

typedef struct s_environment_to_keep_track_of
{
	char		**minienviron;
	t_ast_node	*ast_root;
	int			old_exit_value;
}				t_environ;

enum			e_exit_value
{
	ERROR = -1,
	SUCCESS,
	FAIL
};

//builtin
int				f_pwd(void);
int				f_env(long argc, char **argv);
int				f_echo(long argc, char **argv);
int				f_cd(long argc, char **argv);
int				f_exit(long argc, char **argv);
int				f_exec_pipeline(t_ast_node *pipeline, t_environ *env);
//exe_draft
int				f_execute_ast(t_environ *env);
int				f_exec_logic_expr(t_ast_node *expr, t_environ *env);
void			f_cleanup_pipeline(pid_t *pid_list, int created_processes,
					int temp_fd);
int				f_exec_cmd(t_ast_node *cmd, t_environ *env, int in_pipeline);
int				f_exec_simple_cmd(t_ast_node *simcmd, t_environ *env, int in_pipeline);
char			**f_buildargv(t_ast_node *args, long argc, t_environ *env);
int				f_wait_for_child(pid_t pid);
void			f_cleanup_child(t_ast_node *root, char **argv);
int				f_exec_builtin(long argc, char **argv);
bool			f_is_builtin(char *call);
bool			f_is_builtin_child(char *call);
bool			f_is_builtin_parent(char *call);

#endif