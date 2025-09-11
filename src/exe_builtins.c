/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_builtins.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 00:28:56 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/11 02:13:50 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exe_environment.h"
#include "exe_execution.h"
#define PATHLENGTH 256
#define ENV_ARGC_ERRMSG "minishell: env: too many arguments for this \
	minishell implementation\n"

int	f_handle_n_flag(bool n_flag)
{
	if (!n_flag)
	{
		if (-1 == write(STDOUT_FILENO, "\n", 1))
			return (perror("write"), FAIL);
	}
	return (SUCCESS);
}

int	f_echo(long argc, char **argv)
{
	bool	n_flag;
	long	i;

	if (argc <= 1)
	{
		if (-1 == write(STDOUT_FILENO, "\n", 1))
			return (perror("write"), FAIL);
		return (SUCCESS);
	}
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
	return (f_handle_n_flag(n_flag));
	// if (!n_flag)
	// {
	// 	if (-1 == write(STDOUT_FILENO, "\n", 1))
	// 		return (perror("write"), FAIL);
	// }
	// return (SUCCESS);
}

int	f_pwd(void)
{
	char	*working_dir;

	working_dir = getcwd(NULL, 0);
	if (!working_dir)
		return (perror("getcwd"), FAIL);
	ft_putendl_fd(working_dir, STDOUT_FILENO);
	// printf("%s\n", working_dir);
	free(working_dir);
	return (SUCCESS);
}

int	f_env(char **environ, long argc, bool export_call)
{
	int	i;

	if (argc > 1)
		return (printf(ENV_ARGC_ERRMSG), FAIL);
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
