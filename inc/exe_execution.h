/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_execution.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 00:29:30 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/16 16:07:00 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXE_EXECUTION_H
# define EXE_EXECUTION_H

# include "../libft/libft.h"
# include "identifier.h"
# include "ast_tree_structures.h"
# include "ast_tree_translator.h"
# include "exe_environment.h"
# include "exe_pipeline.h"
# include <fcntl.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>

# define PROMPT "minishell> "
# define FALSE 0
# define TRUE 1
# define IN 0
# define OUT 1

# define ERR_MSG_TOOMANYARGS "too many arguments\n"
# define EXPORT_INV_IDENT "export: `%s': not a valid identifier\n"

extern volatile sig_atomic_t	g_received_signal;

enum			e_exit_value
{
	ERROR = -1,
	SUCCESS = 0,
	GENERAL_ERR = 1,
	MISUSE = 2,
	PERMISSION = 126,
	NOT_FOUND = 127,
	CANCEL = 130,
	QUIT = 131
};

typedef struct s_cmd_ctx
{
	char		**argv;
	long		argc;
	t_ast_node	*redirections;
	bool		error;
	t_env		*env;
	int			in_pipeline;
	int			orig_fds[2];
}				t_cmd_ctx;

// execution tree

int		f_execute_ast(t_env *env);
int		f_exec_builtin(t_cmd_ctx *ctx);
int		f_exec_builtin_parent(t_cmd_ctx *ctx);
int		f_exec_cmd(t_ast_node *cmd, t_env *env, bool in_pipeline);
int		f_exec_external(t_cmd_ctx *ctx);
int		f_exec_simple_cmd(t_ast_node *simcmd, t_env *env, int in_pipeline);

// builtins

int		f_cd(char ***environ, long argc, char **argv);
int		f_echo(long argc, char **argv);
int		f_env(char **environ, long argc, bool export_call);
int		f_exit(long argc, char **argv, bool *error);
int		f_export(char ***environ, long argc, char **argv);
int		f_pwd(void);
int		f_unset(char ***environ, long argc, char **argv);

// utils

void	f_cleanup_child(t_ast_node *root, char **argv);
void	f_create_empty_argv(char ***argv);
int		f_builtin_parent_store_fds(t_cmd_ctx *ctx);
bool	f_is_builtin_parent(char *call);
bool	f_is_builtin_child(char *call);
bool	f_is_builtin(char *call);
int		f_prepare_argv(t_ast_node *simcmd, t_cmd_ctx *ctx);
int		f_wait_for_child(pid_t pid);
void	free_argv(char ***argv);
void	mini_exit(int exit_status, t_env *env);
char	*resolve_executable_path(char **environ, const char *command,
			int *exit_value);

#endif
