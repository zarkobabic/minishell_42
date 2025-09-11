/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_environment_manipulation.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 17:57:35 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/11 18:59:01 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exe_execution.h"

char	*f_environ_find_entry(char **environ, const char *name)
{
	int	i;
	int	name_len;

	if (!environ)
		return (NULL);
	name_len = ft_strlen(name);
	i = -1;
	while (environ[++i])
	{
		if (ft_strncmp(environ[i], name, name_len) == 0
			&& environ[i][name_len] == '=')
			return (environ[i]);
	}
	return (NULL);
}

char	*f_environ_build_entry(const char *name, const char *value)
{
	int		name_len;
	int		val_len;
	char	*new_var;

	name_len = ft_strlen(name);
	val_len = ft_strlen(value);
	new_var = (char *)malloc(name_len + 1 + val_len + 1);
	if (!new_var)
		return (NULL);
	ft_bzero(new_var, name_len + val_len + 2);
	ft_memcpy(new_var, name, name_len);
	new_var[name_len] = '=';
	ft_strlcat(new_var, value, name_len + 1 + val_len + 1);
	return (new_var);
}

int	f_environ_add_entry(char ***environ, char *new_entry)
{
	int		i;
	char	**new_env;

	i = 0;
	while ((*environ)[i])
		++i;
	new_env = (char **)malloc((i + 2) * sizeof(char *));
	if (!new_env)
		return (perror("environ addition malloc"), ERROR);
	i = -1;
	while ((*environ)[++i])
		new_env[i] = (*environ)[i];
	new_env[i] = new_entry;
	new_env[++i] = NULL;
	free(*environ);
	*environ = new_env;
	return (SUCCESS);
}

int	f_environ_del_entry(char ***environ, const char *name)
{
	int		i;
	int		k;
	char	**new_env;
	char	*entry_to_del;

	entry_to_del = f_environ_find_entry(*environ, name);
	if (!entry_to_del)
		return (SUCCESS);
	i = 0;
	while ((*environ)[i])
		++i;
	new_env = (char **)malloc(i * sizeof(char *));
	if (!new_env)
		return (perror("environ del malloc"), ERROR);
	i = 0;
	k = 0;
	while ((*environ)[i])
	{
		if ((*environ)[i] == entry_to_del)
			free((*environ)[i++]);
		else
			new_env[k++] = (*environ)[i++];
	}
	new_env[k] = NULL;
	return (free(*environ), *environ = new_env, SUCCESS);
}
