/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_execution.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 00:29:30 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/08 23:23:08 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXE_EXECUTION_H
# define EXE_EXECUTION_H

# include "../libft/libft.h"
# include "../src/identifier.h"
# include "ast_tree_structures.h"
# include "ast_tree_translator.h"
# include "exe_environment.h"
# include "exe_pipeline.h"
# include <fcntl.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

# define FALSE 0
# define TRUE 1
# define IN 0
# define OUT 1

enum			e_exit_value
{
	ERROR = -1,
	SUCCESS,
	FAIL
};

typedef struct s_cmd_ctx
{
	char		**argv;
	long		argc;
	bool		error;
	t_environ	*env;
	int			in_pipeline;
	int			orig_fds[2];
}	t_cmd_ctx;

typedef struct s_pipeline_ctx
{
	pid_t	*pid_list;
	int		pipe_fds[2];
	int		current_index;
	int		temp_fd_in;
}			t_pipeline_ctx;

void	mini_exit(int exit_status, t_environ *env);

//builtin
int	f_cd(char ***environ, long argc, char **argv);
int	f_echo(long argc, char **argv);
int	f_env(char **environ, long argc, bool export_call);
int	f_exit(long argc, char **argv, bool *error);
int	f_export(char ***environ, long argc, char **argv);
int	f_pwd(void);
int	f_unset(char ***environ, long argc, char **argv);

//send this to pipeline
int				f_exec_pipeline(t_ast_node *pipeline, t_environ *env);

//exe_draft
int				f_execute_ast(t_environ *env);
int				f_exec_logic_expr(t_ast_node *expr, t_environ *env);
void			f_cleanup_pipeline(pid_t *pid_list, int created_processes,
					int temp_fd);
int				f_exec_cmd(t_ast_node *cmd, t_environ *env, bool in_pipeline);
int				f_exec_simple_cmd(t_ast_node *simcmd, t_environ *env,
					int in_pipeline);
char			**f_buildargv(t_ast_node *args, long *argc, t_environ *env);
int				f_wait_for_child(pid_t pid);
void			f_cleanup_child(t_ast_node *root, char **argv);
int				f_exec_builtin(t_cmd_ctx *ctx);
bool			f_is_builtin(char *call);
bool			f_is_builtin_child(char *call);
bool			f_is_builtin_parent(char *call);

#endif