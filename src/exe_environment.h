/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_environment.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 17:21:41 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/12 07:47:17 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXE_ENVIRONMENT_H
# define EXE_ENVIRONMENT_H

# include "ast_tree_structures.h"
# include <stdbool.h>

# define ENV_DELETE_ALL -1
# define ENV_DEFAULT_HOME "/"
# define ENV_DEFAULT_PATH "/usr/local/bin:/usr/bin:/bin"

typedef struct s_environment_to_keep_track_of
{
	char		**minienviron;
	t_ast_node	*ast_root;
	int			old_exit_value;
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
