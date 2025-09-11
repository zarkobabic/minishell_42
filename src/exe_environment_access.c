/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_environment_access.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 22:02:20 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/11 04:36:11 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exe_execution.h"
#include "exe_environment.h"

static void	f_fill_defaults(char ***minienviron)
{
	char	*temp;

	temp = getcwd(NULL, 0);
	if (!f_getenv(*minienviron, "PATH"))
		f_setenv(minienviron, "PATH", ENV_DEFAULT_PATH, true);
	if (!f_getenv(*minienviron, "HOME"))
		f_setenv(minienviron, "HOME", temp, true);
	if (!f_getenv(*minienviron, "PWD"))
		f_setenv(minienviron, "PWD", temp, true);
	free(temp);
}

int	f_build_environ(char ***minienviron)
{
	extern char	**environ;
	int			i;

	i = 0;
	while (environ[i])
		++i;
	*minienviron = (char **)malloc((i + 1) * sizeof(char *));
	if (!minienviron)
		return (perror("environ creation malloc"), ERROR);
	i = -1;
	while (environ[++i])
	{
		(*minienviron)[i] = ft_strdup(environ[i]);
		if (!(*minienviron)[i])
			return (f_destroy_environ(minienviron, i),
				perror("environ creation copy"), ERROR);
	}
	(*minienviron)[i] = NULL;
	f_fill_defaults(minienviron);
	return (SUCCESS);
}

/// @brief Pass entryno = ENV_DELETE_ALL (-1) to free everyhting 
/// (unsigned underflow to high number, so basically infinitely high)
/// @param environ 
/// @param entryno 
void	f_destroy_environ(char ***environ, unsigned int entryno)
{
	unsigned int	i;

	i = -1;
	while (++i < entryno && (*environ)[i])
		free((*environ)[i]);
	free(*environ);
}

char	*f_getenv(char **environ, const char *name)
{
	char	*entry;
	char	*value;

	entry = f_environ_find_entry(environ, name);
	if (!entry)
		return (NULL);
	value = ft_strchr(entry, '=') + 1;
	return (value);
}

int	f_setenv(char ***environ, const char *name, const char *value,
		bool overwrite)
{
	int			i;
	char		*new_var;
	char		*entry_to_set;

	new_var = f_environ_build_entry(name, value);
	entry_to_set = f_environ_find_entry(*environ, name);
	if (!entry_to_set)
		return (f_environ_add_entry(environ, new_var));
	i = -1;
	while ((*environ)[++i])
	{
		if ((*environ)[i] == entry_to_set)
		{
			if (overwrite)
				return (free((*environ)[i]), (*environ)[i] = new_var, SUCCESS);
			else
				return (free(new_var), FAIL);
		}
	}
	return (perror("setenv"), ERROR);
}
