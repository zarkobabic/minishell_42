/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_builtins_parent.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 09:17:57 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/14 10:59:20 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exe_environment.h"
#include "exe_execution.h"

static int	f_environ_create_entry(char ***environ, char *arg)
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
		exit_value = f_environ_create_entry(environ, argv[i]);
		if (exit_value != SUCCESS)
			return (exit_value);
	}
	if (failed)
		return (GENERAL_ERR);
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

static bool	f_isnumeric(char *str)
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
