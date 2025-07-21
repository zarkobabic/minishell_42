/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 00:28:56 by eberkau           #+#    #+#             */
/*   Updated: 2025/07/21 06:31:29 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

#define PATHLENGTH 128
#define ENV_NAME_LENGTH 32

int	f_echo(const char *input, int fd, int n_flag)
{
	// int	fd;
	
	// FIXME: either handled here or handled already in calling mother function
	// fd = 1;
	// if (redirec_flag)
	// 	fd = open(filename, O_WRONLY);


	if (n_flag)
		ft_putstr_fd(input, fd);
	else
		ft_putendl_fd(input, fd);

	// or

	ft_putstr(input, fd);
	if (!n_flag)
		write(fd, "\n", 1);


	// if (redirec_flag)
	// 	close(fd);
	return (0);
}

void	f_cd(const char *path)
{
	char	custom_path[PATHLENGTH];
	char	*user_str;
	char	*home_str;
	int		usr_len;
	int		home_len;

	home_str = getenv("HOME");
	if (!path || (path[0] == '~' && path[1] == '\0'))
	{
		chdir(home_str);
		// TODO: update PWD and OLDPWD env vars
	}
	else if (path[0] == '~' && path[1] != '\0' && path[1] != '/')
	{
		// change to different user's home dir
		user_str = getenv("USER");
		home_len = ft_strlen(home_str);
		usr_len = ft_strlen(user_str);
		ft_strlcat(custom_path, home_str, PATHLENGTH);
		custom_path[home_len - usr_len] = '\0';
		ft_strlcat(custom_path, path + 1, PATHLENGTH);
		chdir(custom_path);
		// TODO: update PWD and OLDPWD env vars
	}
	else if (path[0] == '~' && path[1] != '\0')
	{
		ft_strlcat(custom_path, home_str, PATHLENGTH);
		ft_strlcat(custom_path, path + 1, PATHLENGTH);
		chdir(custom_path);
		// TODO: update PWD and OLDPWD env vars
	}
	else
	{
		chdir(path);
		// TODO: update PWD and OLDPWD env vars
	}
}

void	f_pwd(void)
{
	char	*working_dir;
	
	working_dir = getenv("PWD");
	printf("%s\n", working_dir);
}

void	f_env(void)
{
	char *env_path;
	char env_args[1][ENV_NAME_LENGTH];

	env_path = getenv("_");
	ft_strlcat(env_args[0], ft_strrchr(env_path, '/') + 1, ENV_NAME_LENGTH);
	execve(env_path, env_args, NULL);
}

void	f_exit(void)
{
	f_close_fds(); // TODO (maybe even more cleanup, needs to be evaluated)
	exit (0);
}
