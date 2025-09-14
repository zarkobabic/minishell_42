/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_path_resolution.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 00:39:51 by zbabic            #+#    #+#             */
/*   Updated: 2025/09/14 02:44:13 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "exe_execution.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

bool	f_check_is_directory(const char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) == 0)
	{
		if (S_ISDIR(path_stat.st_mode))
			return (true);
		else
			return (false);
	}
	return (false);
}

char	*handle_absolute_relative_path(const char *command, int *exit_value)
{
	long	command_len;
	char	*full_path;

	if (access(command, X_OK) == 0)
	{
		command_len = ft_strlen(command);
		full_path = malloc(command_len + 1);
		if (!full_path)
			return (NULL);
		ft_memcpy(full_path, command, command_len);
		full_path[command_len] = '\0';
		return (full_path);
	}
	*exit_value = PERMISSION;
	if (f_check_is_directory(command))
		return (ft_putstr_fd_fail_safe((char*)command, STDERR_FILENO),
			ft_putstr_fd_fail_safe(": Is a directory\n", STDERR_FILENO), NULL);
	else
		return (perror(command), NULL);
}

char	*check_is_executable_dir_cmd_path(char *curr_dir_path, long dir_str_len,
		const char *command)
{
	char	*full_path;
	long	need_slash;
	long	command_len;

	command_len = ft_strlen(command);
	need_slash = 0;
	if (curr_dir_path[dir_str_len - 1] != '/')
		need_slash = 1;
	full_path = (char *)malloc(dir_str_len + need_slash + command_len + 1);
	if (!full_path)
		return (NULL);
	ft_memcpy(full_path, curr_dir_path, dir_str_len);
	if (need_slash)
		full_path[dir_str_len] = '/';
	ft_memcpy(full_path + dir_str_len + need_slash, command, command_len);
	full_path[dir_str_len + need_slash + command_len] = '\0';
	if (access(full_path, X_OK) == 0)
		return (full_path);
	free(full_path);
	return (NULL);
}

char	*handle_executable_from_path_env(const char *command, char *path_cpy)
{
	char	*curr_dir_path;
	char	*next_colon_pos;
	long	dir_str_len;
	char	*cmd_full_path;

	curr_dir_path = path_cpy;
	while (curr_dir_path && *curr_dir_path)
	{
		next_colon_pos = ft_strchr(curr_dir_path, ':');
		if (next_colon_pos)
			*next_colon_pos = '\0';
		dir_str_len = ft_strlen(curr_dir_path);
		if (dir_str_len > 0)
		{
			cmd_full_path = check_is_executable_dir_cmd_path(
				curr_dir_path, dir_str_len, command);
			if (cmd_full_path)
				return (free(path_cpy), cmd_full_path);
		}
		if (next_colon_pos)
			curr_dir_path = next_colon_pos + 1;
		else
			curr_dir_path = NULL;
	}
	return (free(path_cpy), NULL);
}

/// @brief Returns NULL in case of error, not executable or doesnt exist,
/// returns string that you can and should free, doesnt free command argument
/// @param command
/// @return
char	*resolve_executable_path(char **environ, const char *command,
		int *exit_value)
{
	char	*path_env;
	long	path_len;
	char	*path_cpy;
	char	*res_path;

	if (ft_strchr(command, '/') != NULL)
		return (handle_absolute_relative_path(command, exit_value));
	path_env = f_getenv(environ, "PATH");
	if (!path_env)
		return (NULL);
	path_len = ft_strlen(path_env);
	path_cpy = (char *)malloc((path_len + 1) * sizeof(char));
	if (!path_cpy)
		return (NULL);
	ft_memcpy(path_cpy, path_env, path_len);
	path_cpy[path_len] = '\0';
	res_path = handle_executable_from_path_env(command, path_cpy);
	if (!res_path)
	{
		*exit_value = NOT_FOUND;
		ft_putstr_fd((char *)command, STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
	}
	return (res_path);
}
