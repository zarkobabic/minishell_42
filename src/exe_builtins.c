/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_builtins.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 00:28:56 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/08 20:44:42 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exe_environment.h"
#include "exe_execution.h"
#define PATHLENGTH 256
// #define ENV_NAME_LENGTH 64

//FINE
int	f_echo(long argc, char **argv)
{
	bool	n_flag;
	long	i;

	if (argc <= 1)
		return (SUCCESS);
	n_flag = !(bool)ft_strncmp(argv[1], "-n", 3);
	i = 1 + n_flag;
	while (i < argc)
	{
		if (-1 == ft_putstr_fd_fail_safe(argv[i], STDOUT_FILENO))
			return (perror("write"), FAIL);
		if (i + 1 < argc)
		{
			if (-1 == write(STDOUT_FILENO, " ", 1))
				return (perror("write"), FAIL);
		}
		++i;
	}
	if (!n_flag)
	{
		if (-1 == write(STDOUT_FILENO, "\n", 1))
			return (perror("write"), FAIL);
	}
	return (SUCCESS);
}

// int	f_cd(char ***environ, long argc, char **argv)
// {
// 	char	custom_path[PATHLENGTH];
// 	char	*temp_str;
// 	int		user_len;
// 	int		home_len;

// 	if (argc > 2)
// 		return (printf("minishell: cd: too many arguments\n"), FAIL);
// 	ft_bzero(custom_path, PATHLENGTH);
// 	if (!argv[1] || (argv[1][0] == '~' && argv[1][1] == '\0'))
// 	{
// 		temp_str = f_getenv(*environ, "HOME");
// 		if(!temp_str)
// 			return SUCCESS;
// 	}
// 	else if (argv[1][0] == '~' && argv[1][1])
// 	{
// 		temp_str = f_getenv(*environ, "HOME");
// 		if(!temp_str)
// 			return SUCCESS;
// 		ft_strlcat(custom_path, temp_str, PATHLENGTH);
// 		ft_strlcat(custom_path, &(argv[1][1]), PATHLENGTH);
// 		temp_str = custom_path;
// 	}
// 	else if (argv[1][0] == '-' && !argv[1][1])
// 	{
// 		temp_str = f_getenv(*environ, "OLDPWD");
// 		if(!temp_str)
// 			return SUCCESS;
// 		printf("%s\n", temp_str);
// 	}
// 	else
// 		temp_str = argv[1];
// 	if (chdir(temp_str) == -1)
// 		return (perror("minishell: cd"), FAIL);
// 	f_setenv(environ, "OLDPWD", f_getenv(*environ, "PWD"), true);
// 	temp_str = getcwd(NULL, 0);
// 	f_setenv(environ, "PWD", temp_str, true);
// 	free(temp_str);
// 	return (SUCCESS);
// }

int	f_pwd(void)
{
	char	*working_dir;

	working_dir = getcwd(NULL, 0);
	if (!working_dir)
		return (perror("getcwd"), FAIL);
	printf("%s\n", working_dir);
	free(working_dir);
	return (SUCCESS);
}

int	f_env(char **environ, long argc, bool export_call)
{
	int	i;

	if (argc > 1)
		return (printf("minishell: env: too many arguments for this "
						"minishell implementation\n"),
				FAIL);
	i = -1;
	while (environ[++i])
	{
		if (export_call)
			printf("declare -x ");
		printf("%s\n", environ[i]);
	}
	return (SUCCESS);
}

int	f_export(char ***environ, long argc, char **argv)
{
	int		i;
	int		exit_value;
	char	*name;
	char	*value;
	char	*eq_sign;

	if (argc == 1)
		return (f_env(*environ, 1, true));
	i = 0;
	while (++i < argc)
	{
		eq_sign = ft_strchr(argv[i], '=');
		if (!eq_sign)
			continue ;
		*eq_sign = '\0';
		name = argv[i];
		value = eq_sign + 1;
		exit_value = f_setenv(environ, name, value, true);
		if (exit_value != SUCCESS)
			return (exit_value);
	}
	return (SUCCESS);
}

int	f_unset(char ***environ, long argc, char **argv)
{
	int	i;
	int	exit_value;

	if (argc == 1)
		return (SUCCESS);
	i = -1;
	while (++i < argc)
	{
		exit_value = f_environ_del_entry(environ, argv[i]);
		if (exit_value != SUCCESS)
			return (exit_value);
	}
	return (SUCCESS);
}

bool	f_isnumeric(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (false);
		++i;
	}
	return (true);
}

int	f_exit(long argc, char **argv, bool *error)
{
	if (argc > 2)
		return (*error = true, printf("minishell: exit: too many arguments\n"),
			FAIL);
	if (argc == 2 && !f_isnumeric(argv[1]))
		return (*error = true,
			printf("minishell: exit: numeric argument required\n"), FAIL);
	ft_putendl_fd("exit", STDOUT_FILENO);
	if (argc == 2)
		return (ft_atoi(argv[1]));
	else
		return (SUCCESS);
}
