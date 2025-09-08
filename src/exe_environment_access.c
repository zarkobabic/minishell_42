/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_environment_access.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 22:02:20 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/08 19:53:06 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exe_execution.h"

int	f_build_environ(char ***minienviron)
{
	extern char	**environ;
	int			i;

	i = 0;
	while (environ[i])
		++i;
	*minienviron = (char **)malloc((i + 1) * sizeof(char *));
	if (!minienviron)
		return (perror("environ creation malloc"), ERROR);
	i = -1;
	while (environ[++i])
	{
		(*minienviron)[i] = ft_strdup(environ[i]);
		if (!(*minienviron)[i])
			return (f_destroy_environ(minienviron, i),
				perror("environ creation copy"), ERROR);
	}
	(*minienviron)[i] = NULL;
	return (SUCCESS);
}

/// @brief Pass entryno = ENV_DELETE_ALL (-1) to free everyhting 
/// (unsigned underflow to high number, so basically infinitely high)
/// @param environ 
/// @param entryno 
void	f_destroy_environ(char ***environ, unsigned int entryno)
{
	unsigned int	i;

	i = -1;
	while (++i < entryno && (*environ)[i])
		free((*environ)[i]);
	free(*environ);
}

char	*f_getenv(char **environ, const char *name)
{
	char	*entry;
	char	*value;

	entry = f_environ_find_entry(environ, name);
	if (!entry)
		return (NULL);
	value = ft_strchr(entry, '=') + 1;
	return (value);
}

int	f_setenv(char ***environ, const char *name, const char *value,
		bool overwrite)
{
	int			i;
	char		*new_var;
	char		*entry_to_set;

	new_var = f_environ_build_entry(name, value);
	entry_to_set = f_environ_find_entry(*environ, name);
	if (!entry_to_set)
		return (f_environ_add_entry(environ, new_var));
	i = -1;
	while ((*environ)[++i])
	{
		if ((*environ)[i] == entry_to_set)
		{
			if (overwrite)
				return (free((*environ)[i]), (*environ)[i] = new_var, SUCCESS);
			else
				return (free(new_var), FAIL);
		}
	}
	return (perror("setenv"), ERROR);
}

// #include <stdio.h>
// #include <stdlib.h>
// #include <stdbool.h>
// #include <string.h>

// int main(void)
// {
//     t_constants data;
//     char **myenv;
//     int ret;

//     ft_bzero(&data, sizeof(data));
//     myenv = data.minienviron;

//     // Build a copy of the current environment
//     ret = f_build_environ(&myenv);
//     if (ret != SUCCESS) {
//         printf("Failed to build environment\n");
//         return 1;
//     }

//     // Print all environment variables
//     printf("Initial environment:\n");
//     for (int i = 0; myenv && myenv[i]; i++)
//         printf("%s\n", myenv[i]);

//     // Heap-allocate argv for export
//     long argc = 6;
//     char **argv = malloc(sizeof(char *) * argc);
//     if (!argv) {
//         perror("malloc argv");
//         f_destroy_environ(&myenv, (unsigned int)-1);
//         return 1;
//     }
//     argv[0] = strdup("export");
//     argv[1] = strdup("test=xzy");
//     argv[2] = strdup("test3=abccc");
//     argv[3] = strdup("testttt=crazy");
//     argv[4] = strdup("test2");
//     argv[5] = strdup("test666=devil=evil");
//     for (int i = 0; i < argc; i++) {
//         if (!argv[i]) {
//             perror("strdup argv");
//             // Free previously allocated argv entries
//             for (int j = 0; j < i; j++)
//                 free(argv[j]);
//             free(argv);
//             f_destroy_environ(&myenv, (unsigned int)-1);
//             return 1;
//         }
//     }

//     // Add new variables using export
//     printf("\nExporting variables...\n");
//     f_export(&myenv, argc, argv);

//     // Print again
//     for (int i = 0; myenv && myenv[i]; i++)
//         printf("%s\n", myenv[i]);

//     // Change TESTVAR
//     printf("\nChanging TESTVAR=w...\n");
//     f_setenv(&myenv, "TESTVAR", "w", true);

//     // Print again
//     for (int i = 0; myenv && myenv[i]; i++)
//         printf("%s\n", myenv[i]);

//     // Unset TESTVAR
//     printf("\nUnsetting TESTVAR...\n");
//     f_unset(&myenv, 2, (char*[]){"unset", "TESTVAR"});

//     // Print again
//     for (int i = 0; myenv && myenv[i]; i++)
//     	printf("%s\n", myenv[i]);

// printf("\nTESTING double equal signs in export call:\vvalue: 
// 	%s\n", f_getenv(myenv, "test666"));

// printf("\nTesting export call without arguments...\n");
// f_export(&myenv, 1, NULL);

//     // Free environment
//     f_destroy_environ(&myenv, (unsigned int)-1);

//     // Free heap-allocated argv
//     for (int i = 0; i < argc; i++)
//         free(argv[i]);
//     free(argv);

//     return 0;
// }
