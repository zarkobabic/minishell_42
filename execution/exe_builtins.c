/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_builtins.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 00:28:56 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/06 20:30:07 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exe_execution.h"

#define PATHLENGTH 256
#define ENV_NAME_LENGTH 64

// bool	f_found_nflag(char **argv, int argc)
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
	// int	fd;
	
	// FIXME: either handled here or handled already in calling mother function
	// fd = 1;
	// if (redirec_flag)
	// 	fd = open(filename, O_WRONLY);

	bool	n_flag;
	long		i;
	
	n_flag = !(bool)ft_strncmp(argv[1], "-n", 3);
	// n_flag = f_found_nflag(argv, argc);
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
			return (free(environ[i]), environ[i] = new_var, 0);
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

	if (argc > 2)
		return (printf("minishell: cd: too many arguments\n"), FAIL);
	ft_bzero(custom_path, PATHLENGTH);
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
		temp_str = getenv("OLDPWD");
		printf("%s\n", temp_str);
	}
	else
		temp_str = argv[1];
	if (chdir(temp_str) == -1)
		return (perror("minishell: cd"), FAIL);
	f_setenv("OLDPWD", getenv("PWD"));
	f_setenv("PWD", temp_str);
	return (SUCCESS);
}

int	f_pwd(void)
{
	char	*working_dir;
	
	working_dir = getenv("PWD");
	if (!working_dir)
		return (perror("getenv"), FAIL);
	printf("%s\n", working_dir);
	return (SUCCESS);
}

int	f_env(long argc, char **argv)
{
	// char *env_path;
	// char *env_args[2];

	// env_path = "/usr/bin/env";
	// env_args[0] = ft_strrchr(env_path, '/') + 1;
	// env_args[1] = NULL;
	// execve(env_path, env_args, NULL); // FIXME: env_args is of wrong argument type

	// or
	// probably preferred:

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
	if (argc == 2)
		exit(ft_atoi(argv[1]));
	else
		exit (SUCCESS);
	return (SUCCESS);
}

// void	f_call_program(int call)
// {
// 	if (call == CALL_ECHO)
// 		f_echo();
// 	else if (call == CALL_CD)
// 		f_cd();
// 	// etc
// }

// int	f_create_subshell(int program)
// {
// 	pid_t	pid;
// 	int		status;

// 	pid = fork();
// 	if (pid == 0)
// 	{
// 		// do stuff, e.g.:
// 		f_call_program(program); // TODO
// 	}
// 	else if (pid > 0)
// 		waitpid(pid, &status, 0);
// 	else
// 	{
// 		// fork error
// 	}
// 	return (status);
// }

// void	f_setup_func_table(t_funcdict *func_dict)
// { // FIXME: probably not a good approach, function signatures differ -> use hardcoded if-elseif list for function mapping/calling instead
// 	func_dict[CALL_ECHO].call = "echo";
// 	func_dict[CALL_ECHO].func = f_echo;
// 	func_dict[CALL_CD].call = "cd";
// 	func_dict[CALL_CD].func = f_cd;
// 	func_dict[CALL_PWD].call = "pwd";
// 	func_dict[CALL_PWD].func = f_pwd;
// 	func_dict[CALL_EXPORT].call = "export";
// 	func_dict[CALL_EXPORT].func = ;
// 	func_dict[CALL_UNSET].call = "unset";
// 	func_dict[CALL_UNSET].call = ;
// 	func_dict[CALL_ENV].call = "env";
// 	func_dict[CALL_ENV].func = f_env;
// 	func_dict[CALL_EXIT].call = "exit";
// 	func_dict[CALL_EXIT].func = f_exit;




	
// 	// func_table[CALL_ECHO] = f_echo;
// 	// func_table[CALL_CD] = f_cd;
// 	// func_table[CALL_PWD] = f_pwd;
// 	// func_table[CALL_EXPORT] = ;
// 	// func_table[CALL_UNSET] = ;
// 	// func_table[CALL_ENV] = f_env;
// 	// func_table[CALL_EXIT] = f_exit;
// }

// int	main(void)
// {
// 	static t_funcdict func_dict[INPUT_COUNT] = {
// 		{"echo", f_echo},
// 		{"cd", f_cd},
// 		{"pwd", f_pwd},
// 		{"export", },
// 		{"unset", },
// 		{"env", f_env},
// 		{"exit", f_exit}
// 	};
// 	void		(*func_table[INPUT_COUNT])(void);
// 	t_funcdict	func_dict[INPUT_COUNT];

// 	f_setup_func_table(func_dict);
// 	return (0);
// }
