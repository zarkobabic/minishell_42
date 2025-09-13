/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_environment.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 17:21:41 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/13 09:27:19 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXE_ENVIRONMENT_H
# define EXE_ENVIRONMENT_H

# include "ast_tree_structures.h"
# include <stdbool.h>
# include <termios.h>

# define ENV_DELETE_ALL -1
# define ENV_DEFAULT_HOME "/"
# define ENV_DEFAULT_PATH "/usr/local/bin:/usr/bin:/bin"
# define HEREDOC_COUNT 100
# define HEREDOC_NAME_SIZE 16
// # define HEREDOC_NAME_PREFIX "/temp/.r"
# define HEREDOC_NAME_PREFIX "./.tmp/.r"
# define HEREDOC_NAME_SUFFIX ".txt"


typedef struct s_heredoc
{
	t_ast_node *redir_ptr;
	char		name[HEREDOC_NAME_SIZE];
}	t_heredoc;

typedef struct s_environment_to_keep_track_of
{
	char			**minienviron;
	t_ast_node		*ast_root;
	int				old_exit_value;
	t_heredoc		heredoclist[HEREDOC_COUNT];
	struct termios	orig_term;
}				t_env;

// environ table management

int				f_build_environ(char ***minienviron);
void			f_destroy_environ(char ***environ, unsigned int entryno);
int				f_environ_add_entry(char ***environ, char *new_entry);
char			*f_environ_build_entry(const char *name, const char *value);
int				f_environ_del_entry(char ***environ, const char *name);
char			*f_environ_find_entry(char **environ, const char *name);
bool			f_is_valid_name(const char *name);

// environ access

char			*f_getenv(char **environ, const char *name);
int				f_setenv(char ***environ, const char *name, const char *value,
					bool overwrite);

#endif
