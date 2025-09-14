/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_builtins.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 00:28:56 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/14 07:02:27 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exe_environment.h"
#include "exe_execution.h"

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
}

int	f_pwd(void)
{
	char	*working_dir;

	working_dir = getcwd(NULL, 0);
	if (!working_dir)
		return (perror("getcwd"), FAIL);
	ft_putendl_fd(working_dir, STDOUT_FILENO);
	free(working_dir);
	return (SUCCESS);
}

int	f_env(char **environ, long argc, bool export_call)
{
	int	i;

	if (argc > 1)
		return (ft_putstr_fd_fail_safe(ENV_ARGC_ERRMSG, STDERR_FILENO), MISUSE); // FIXME
	i = -1;
	while (environ[++i])
	{
		if (export_call)
			printf("declare -x ");
		printf("%s\n", environ[i]);
	}
	return (SUCCESS);
}

static int	f_process_assignment(char ***environ, char *arg)
{
	char	*value;
	char	*eq_sign;
	int		exit_value;

	eq_sign = ft_strchr(arg, '=');
	if (!eq_sign)
		return (SUCCESS);
	*eq_sign = '\0';
	value = eq_sign + 1;
	exit_value = f_setenv(environ, arg, value, true);
	return (exit_value);
}

int	f_export(char ***environ, long argc, char **argv)
{
	int		i;
	int		exit_value;
	bool	failed;

	if (argc == 1)
		return (f_env(*environ, 1, true));
	failed = false;
	i = 0;
	while (++i < argc)
	{
		if (!f_is_valid_name(argv[i]))
		{
			ft_putstr_fd_fail_safe("export: ", STDERR_FILENO);
			ft_putstr_fd_fail_safe(argv[i], STDERR_FILENO);
			ft_putstr_fd_fail_safe(": not a valid identifier\n", STDERR_FILENO);
			failed = true;
			continue ;
		}
		exit_value = f_process_assignment(environ, argv[i]);
		if (exit_value != SUCCESS)
			return (exit_value);
	}
	if (failed)
		return (GENERAL);
	return (SUCCESS);
}

// int	f_export(char ***environ, long argc, char **argv)
// {
// 	int		i;
// 	int		exit_value;
// 	char	*value;
// 	char	*eq_sign;
// 	bool	failed;

// 	failed = false;
// 	if (argc == 1)
// 		return (f_env(*environ, 1, true));
// 	i = 0;
// 	while (++i < argc)
// 	{
// 		if (!f_is_valid_name(argv[i]))
// 		{
// 			printf(EXPORT_INV_IDENT, argv[i]);
// 			failed = true;
// 			continue ;
// 		}
// 		eq_sign = ft_strchr(argv[i], '=');
// 		if (!eq_sign)
// 			continue ;
// 		*eq_sign = '\0';
// 		value = eq_sign + 1;
// 		exit_value = f_setenv(environ, argv[i], value, true);
// 		if (exit_value != SUCCESS)
// 			return (exit_value);
// 	}
// 	if(failed)
// 		return (GENERAL);
// 	return (SUCCESS);
// }

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
	if (!str[i])
		return (false);
	while (str[i])
	{
		if (!(ft_isdigit(str[i]) || (i == 0 && (str[i] == '-'
						|| str[i] == '+'))))
			return (false);
		++i;
	}
	return (true);
}

int	f_exit(long argc, char **argv, bool *error)
{
	ft_putendl_fd("exit", STDERR_FILENO);
	if (argc > 1 && !f_isnumeric(argv[1]))
		return (*error = true,
			ft_putstr_fd_fail_safe(
				"exit: numeric argument required\n", STDERR_FILENO),
			MISUSE);
	if (argc > 2)
		return (*error = true,
			ft_putstr_fd_fail_safe(
				"exit: " ERR_MSG_TOOMANYARGS, STDERR_FILENO),
			MISUSE);
	if (argc == 2)
		return (ft_atoi(argv[1]));
	else
		return (SUCCESS);
}
