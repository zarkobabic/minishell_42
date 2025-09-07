/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_builtins.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 00:28:56 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/08 01:03:51 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exe_execution.h"

#define PATHLENGTH 256
#define ENV_NAME_LENGTH 64

// bool	f_found_nflag(long argc, char **argv)
// {
// 	int	i;

// 	i = 0;
// 	while (i < 0)
// 	{
// 		if (!ft_strncmp(argv[i], "-n", 3))
// 			return (true);
// 		++i;
// 	}
// 	return (false);
// }

int	f_echo(long argc, char **argv)
{
	bool	n_flag;
	long		i;
	
	if(argc <= 1)
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

int	f_setenv(const char *name, char *value)
{
	extern char	**environ;
	int			name_len;
	int			val_len;
	int			i;
	char		*new_var;

	name_len = ft_strlen(name);
	val_len = ft_strlen(value);
	new_var = (char *)malloc(name_len + 1 + val_len + 1);
	if (!new_var)
		return (1);
	new_var[0] = '\0';
	ft_strlcat(new_var, name, name_len + 1);
	new_var[name_len] = '=';
	ft_strlcat(new_var, value, name_len + 1 + val_len + 1);
	i = -1;
	while (environ[++i])
	{
		if (0 == ft_strncmp(environ[i], name, name_len)
		&& environ[i][name_len] == '=')
			return (/* free(environ[i]),  */environ[i] = new_var, 0);
	}
	if (!environ[i])
		free(new_var);
	return (1);
}

int	f_cd(long argc, char **argv)
{
	char	custom_path[PATHLENGTH];
	char	*temp_str;
	int		user_len;
	int		home_len;
	bool	alloc_flag;

	if (argc > 2)
		return (printf("minishell: cd: too many arguments\n"), FAIL);
	ft_bzero(custom_path, PATHLENGTH);
	alloc_flag = false;
	if (!argv[1] || (argv[1][0] == '~' && argv[1][1] == '\0'))
		temp_str = getenv("HOME");
	else if (argv[1][0] == '~' && argv[1][1] != '\0' && argv[1][1] != '/')
	{
		// change to different user's home dir
		temp_str = getenv("USER");
		user_len = ft_strlen(temp_str);
		temp_str = getenv("HOME");
		home_len = ft_strlen(temp_str);
		ft_strlcat(custom_path, temp_str, PATHLENGTH);
		custom_path[home_len - user_len] = '\0';
		ft_strlcat(custom_path, &(argv[1][1]), PATHLENGTH);
		temp_str = custom_path;
	}
	else if (argv[1][0] == '~' && argv[1][1])
	{
		temp_str = getenv("HOME");
		ft_strlcat(custom_path, temp_str, PATHLENGTH);
		ft_strlcat(custom_path, &(argv[1][1]), PATHLENGTH);
		temp_str = custom_path;
	}
	else if (argv[1][0] == '-' && !argv[1][1])
	{
		temp_str = ft_strdup(getenv("OLDPWD"));
		alloc_flag = true;
		printf("%s\n", temp_str);
	}
	else
		temp_str = argv[1];
	if (chdir(temp_str) == -1)
		return (perror("minishell: cd"), FAIL);
	f_setenv("OLDPWD", getenv("PWD"));
	f_setenv("PWD", temp_str);
	if (alloc_flag)
		free(temp_str);
	return (SUCCESS);
}

//FINE
int	f_pwd(void)
{
	char	*working_dir;
	
	working_dir = getenv("PWD");
	if (!working_dir)
		return (perror("getenv"), FAIL);
	printf("%s\n", working_dir);
	return (SUCCESS);
}

//FINE
int	f_env(long argc, char **argv)
{
	extern char	**environ;
	int			i;

	(void)argv;
	if (argc > 1)
		return (printf("minishell: env: too many arguments for this minishell implementation\n"), FAIL);
	i = -1;
	while (environ[++i])
		printf("%s\n", environ[i]);
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

int	f_exit(long argc, char **argv/* , t_ast_node *ast_root */)
{
	if (argc > 2)
		return (printf("minishell: exit: too many arguments\n"), FAIL);
	if (argc == 2 && !f_isnumeric(argv[1]))
		return (printf("minishell: exit: numeric argument required\n"), FAIL);
	// f_close_fds(); // TODO (more cleanup, needs to be evaluated)
	ft_putendl_fd("exit", STDOUT_FILENO);
	if (argc == 2)
		exit(ft_atoi(argv[1]));
	else
		exit (SUCCESS);
	return (SUCCESS);
}