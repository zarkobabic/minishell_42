/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_builtins_cd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 20:40:38 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/08 23:30:20 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exe_environment.h"
#include "exe_execution.h"
#define PATHLENGTH 256

static char	*f_handle_home_directory(char ***environ)
{
	char	*home_path;

	home_path = f_getenv(*environ, "HOME");
	if (!home_path)
		return (NULL);
	return (home_path);
}

static char	*f_handle_tilde_path(char ***environ, char *arg, char *custom_path)
{
	char	*home_path;

	home_path = f_getenv(*environ, "HOME");
	if (!home_path)
		return (NULL);
	ft_strlcat(custom_path, home_path, PATHLENGTH);
	ft_strlcat(custom_path, &(arg[1]), PATHLENGTH);
	return (custom_path);
}

static char	*f_handle_oldpwd(char ***environ)
{
	char	*oldpwd_path;

	oldpwd_path = f_getenv(*environ, "OLDPWD");
	if (!oldpwd_path)
		return (NULL);
	printf("%s\n", oldpwd_path);
	return (oldpwd_path);
}

static int	f_change_directory_and_update_env(char ***environ, char *path)
{
	char	*current_pwd;
	char	*new_pwd;

	if (chdir(path) == -1)
		return (perror("minishell: cd"), FAIL);
	current_pwd = f_getenv(*environ, "PWD");
	f_setenv(environ, "OLDPWD", current_pwd, true);
	new_pwd = getcwd(NULL, 0);
	f_setenv(environ, "PWD", new_pwd, true);
	free(new_pwd);
	return (SUCCESS);
}

int	f_cd(char ***environ, long argc, char **argv)
{
	char	custom_path[PATHLENGTH];
	char	*target_path;

	if (argc > 2)
		return (printf("minishell: cd: too many arguments\n"), FAIL);
	ft_bzero(custom_path, PATHLENGTH);
	if (!argv[1] || (argv[1][0] == '~' && argv[1][1] == '\0'))
		target_path = f_handle_home_directory(environ);
	else if (argv[1][0] == '~' && argv[1][1])
		target_path = f_handle_tilde_path(environ, argv[1], custom_path);
	else if (argv[1][0] == '-' && !argv[1][1])
		target_path = f_handle_oldpwd(environ);
	else
		target_path = argv[1];
	if (!target_path)
		return (SUCCESS);
	return (f_change_directory_and_update_env(environ, target_path));
}
