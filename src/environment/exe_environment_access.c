/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_environment_access.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 22:02:20 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/14 12:12:03 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exe_execution.h"
#include "exe_environment.h"

int	f_environ_build(char ***minienviron)
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
			return (f_environ_destroy(minienviron, i),
				perror("environ creation copy"), ERROR);
	}
	(*minienviron)[i] = NULL;
	f_environ_fill_default(minienviron);
	return (SUCCESS);
}

/// @brief Pass entryno = ENV_DELETE_ALL (-1) to free everyhting 
/// (unsigned underflow to high number, so basically infinitely high)
/// @param environ 
/// @param entryno 
void	f_environ_destroy(char ***environ, unsigned int entryno)
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

bool	f_is_valid_name(const char *name)
{
	int	i;

	if (!name || !(ft_isalpha(name[0]) || name[0] == '_'))
		return (false);
	i = 1;
	while (name[i] && name[i] != '=')
	{
		if (!(ft_isalnum(name[i]) || name[i] == '_'))
			return (false);
		++i;
	}
	return (true);
}

int	f_setenv(char ***environ, const char *name, const char *value,
		bool overwrite)
{
	int			i;
	char		*new_var;
	char		*entry_to_set;

	if (!f_is_valid_name(name))
		return (GENERAL_ERR);
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
				return (free(new_var), GENERAL_ERR);
		}
	}
	return (perror("setenv"), ERROR);
}
