/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   identifier_var_expansion.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 21:04:03 by zbabic            #+#    #+#             */
/*   Updated: 2025/09/14 14:01:44 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "exe_environment.h"
#include "identifier.h"
#include <stdbool.h>
#include <stdlib.h>

long	count_env_var(char *str, long var_name_len, t_env *env)
{
	char	*var_name;
	char	*sys_env_string;

	if (str[0] == '?' && var_name_len == 1)
		return (count_digits(env->old_exit_value));
	var_name = (char *)malloc(var_name_len + 1);
	if (!var_name)
		return (-1);
	var_name = ft_memcpy(var_name, str, var_name_len);
	var_name[var_name_len] = '\0';
	sys_env_string = f_getenv(env->minienviron, var_name);
	free(var_name);
	return ((long)(ft_strlen(sys_env_string)));
}

// We have error_flag here because functions can return NULL
// as valid value when returning the value for the
// environment variable that is not set
char	*get_env_var_value(char *str, long var_name_len, bool *error_flag,
		t_env *env)
{
	char	*var_name;
	char	*sys_env_string;

	var_name = (char *)malloc(var_name_len + 1);
	if (!var_name)
		return (*error_flag = true, NULL);
	var_name = ft_memcpy(var_name, str, var_name_len);
	var_name[var_name_len] = '\0';
	sys_env_string = f_getenv(env->minienviron, var_name);
	free(var_name);
	return (sys_env_string);
}

void	copy_regular_char(char c, char *new_str, long *new_cnt,
		bool *in_quotes)
{
	new_str[*new_cnt] = c;
	++(*new_cnt);
	if (c == '\'' && !in_quotes[DOUBLE_QUOTES])
		in_quotes[SINGLE_QUOTES] = !in_quotes[SINGLE_QUOTES];
	else if (c == '\"' && !in_quotes[SINGLE_QUOTES])
		in_quotes[DOUBLE_QUOTES] = !in_quotes[DOUBLE_QUOTES];
}

/// @brief Returns NULL if the allocation failed; do not free input string
/// @param str do not free this, because they are part of ast
/// @return
char	*identifier_expand_variables_in_string(char *str, t_env *env,
		bool heredoc_single_quotes)
{
	long	new_size;
	char	*new_str;

	if (!str)
		return (NULL);
	new_size = identifier_count_new_string_len(str, env, heredoc_single_quotes);
	if (new_size < 0)
		return (NULL);
	new_str = identifier_expand_environment_variables(str, new_size, env,
			heredoc_single_quotes);
	if (!new_str)
		return (NULL);
	return (new_str);
}
