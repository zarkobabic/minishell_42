/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_builtins.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 00:28:56 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/14 11:01:23 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exe_environment.h"
#include "exe_execution.h"

static int	f_handle_n_flag(bool n_flag)
{
	if (!n_flag)
	{
		if (-1 == write(STDOUT_FILENO, "\n", 1))
			return (perror("write"), GENERAL_ERR);
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
			return (perror("write"), GENERAL_ERR);
		return (SUCCESS);
	}
	n_flag = !(bool)ft_strncmp(argv[1], "-n", 3);
	i = 1 + n_flag;
	while (i < argc)
	{
		if (-1 == ft_putstr_fd_fail_safe(argv[i], STDOUT_FILENO))
			return (perror("write"), GENERAL_ERR);
		if (i + 1 < argc)
		{
			if (-1 == write(STDOUT_FILENO, " ", 1))
				return (perror("write"), GENERAL_ERR);
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
		return (perror("getcwd"), GENERAL_ERR);
	ft_putendl_fd(working_dir, STDOUT_FILENO);
	free(working_dir);
	return (SUCCESS);
}

int	f_env(char **environ, long argc, bool export_call)
{
	int	i;

	if (argc > 1)
		return (ft_putstr_fd_fail_safe("env : " ERR_MSG_TOOMANYARGS,
				STDERR_FILENO), MISUSE);
	i = -1;
	while (environ[++i])
	{
		if (export_call)
			printf("declare -x ");
		printf("%s\n", environ[i]);
	}
	return (SUCCESS);
}
