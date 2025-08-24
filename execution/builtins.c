/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 00:28:56 by eberkau           #+#    #+#             */
/*   Updated: 2025/08/24 21:18:33 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

#define PATHLENGTH 128
#define ENV_NAME_LENGTH 32

int	f_echo(char *input, int fd, int n_flag)
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

	ft_putstr_fd(input, fd);
	if (!n_flag)
		write(fd, "\n", 1);


	// if (redirec_flag)
	// 	close(fd);
	return (0);
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

void	f_cd(char *path)
{
	char	custom_path[PATHLENGTH];
	char	*temp_str;
	int		user_len;
	int		home_len;

	ft_bzero(custom_path, PATHLENGTH);
	if (!path || (path[0] == '~' && !path[1]))
		temp_str = getenv("HOME");
	else if (path[0] == '~' && path[1] && path[1] != '/')
	{
		// change to different user's home dir
		temp_str = getenv("USER");
		user_len = ft_strlen(temp_str);
		temp_str = getenv("HOME");
		home_len = ft_strlen(temp_str);
		ft_strlcat(custom_path, temp_str, PATHLENGTH);
		custom_path[home_len - user_len] = '\0';
 		ft_strlcat(custom_path, path + 1, PATHLENGTH);
		temp_str = custom_path;
	}
	else if (path[0] == '~' && path[1])
	{
		temp_str = getenv("HOME");
		ft_strlcat(custom_path, temp_str, PATHLENGTH);
		ft_strlcat(custom_path, path + 1, PATHLENGTH);
		temp_str = custom_path;
	}
	else if (path[0] == '-' && !path[1])
	{
		temp_str = getenv("OLDPWD");
		printf("%s\n", temp_str);
	}
	else
		temp_str = path;
	if (chdir(temp_str) == -1)
		return (perror("minishell: cd"), (void)temp_str);
	f_setenv("OLDPWD", getenv("PWD"));
	f_setenv("PWD", temp_str);
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
	char *env_args[2];

	env_path = "/usr/bin/env";
	env_args[0] = ft_strrchr(env_path, '/') + 1;
	env_args[1] = NULL;
	execve(env_path, env_args, NULL); // FIXME: env_args is of wrong argument type

	// or
	// probably preferred:

	extern char	**environ;
	int			i;

	i = -1;
	while (environ[++i])
		printf("%s\n", environ[i]);
}

void	f_exit(void)
{
	f_close_fds(); // TODO (maybe even more cleanup, needs to be evaluated)
	exit (0);
}

void	f_call_program(int call)
{
	if (call == CALL_ECHO)
		f_echo();
	else if (call == CALL_CD)
		f_cd();
	// etc
}

int	f_create_subshell(int program)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		// do stuff, e.g.:
		f_call_program(program); // TODO
	}
	else if (pid > 0)
		waitpid(pid, &status, 0);
	else
	{
		// fork error
	}
	return (status);
}

void	f_setup_func_table(t_funcdict *func_dict)
{ // FIXME: probably not a good approach, function signatures differ -> use hardcoded if-elseif list for function mapping/calling instead
	func_dict[CALL_ECHO].call = "echo";
	func_dict[CALL_ECHO].func = f_echo;
	func_dict[CALL_CD].call = "cd";
	func_dict[CALL_CD].func = f_cd;
	func_dict[CALL_PWD].call = "pwd";
	func_dict[CALL_PWD].func = f_pwd;
	func_dict[CALL_EXPORT].call = "export";
	func_dict[CALL_EXPORT].func = ;
	func_dict[CALL_UNSET].call = "unset";
	func_dict[CALL_UNSET].call = ;
	func_dict[CALL_ENV].call = "env";
	func_dict[CALL_ENV].func = f_env;
	func_dict[CALL_EXIT].call = "exit";
	func_dict[CALL_EXIT].func = f_exit;




	
	// func_table[CALL_ECHO] = f_echo;
	// func_table[CALL_CD] = f_cd;
	// func_table[CALL_PWD] = f_pwd;
	// func_table[CALL_EXPORT] = ;
	// func_table[CALL_UNSET] = ;
	// func_table[CALL_ENV] = f_env;
	// func_table[CALL_EXIT] = f_exit;
}

int	main(void)
{
	static t_funcdict func_dict[INPUT_COUNT] = {
		{"echo", f_echo},
		{"cd", f_cd},
		{"pwd", f_pwd},
		{"export", },
		{"unset", },
		{"env", f_env},
		{"exit", f_exit}
	};
	void		(*func_table[INPUT_COUNT])(void);
	t_funcdict	func_dict[INPUT_COUNT];

	f_setup_func_table(func_dict);
	return (0);
}
