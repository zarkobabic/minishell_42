/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   identifier_var_expansion.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 21:04:03 by zbabic            #+#    #+#             */
/*   Updated: 2025/09/11 20:15:24 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "exe_environment.h"
#include <stdbool.h>
#include <stdlib.h>

bool	check_env_var_naming(char *str)
{
	if (!str)
		return (false);
	if (str[0] == '?')
		return (true);
	if (!(ft_isalpha(str[0]) || str[0] == '_'))
		return (false);
	return (true);
}

long	get_env_var_name_len(char *str)
{
	long	i;

	if (!str || !check_env_var_naming(str))
		return (0);
	i = 0;
	if (str[0] == '?')
		return (1);
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		++i;
	return (i);
}

// Cannot be negative in bash
int	count_digits(int num)
{
	int	digits_cnt;

	digits_cnt = 0;
	if (num == 0)
		return (1);
	while (num)
	{
		++digits_cnt;
		num = num / 10;
	}
	return (digits_cnt);
}

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

static long	handle_env_var_expansion(char *str, long *i, long *char_cnt, t_env *env)
{
	long	var_name_len;
	long	var_cnt;

	var_name_len = get_env_var_name_len(&str[*i + 1]);
	if (var_name_len > 0)
	{
		++(*i);
		var_cnt = count_env_var(&str[*i], var_name_len, env);
		if (var_cnt < 0)
			return (var_cnt);
		*char_cnt += var_cnt;
		*i += (var_name_len - 1);
	}
	else
		*char_cnt += 1;
	return (0);
}

static void	handle_regular_char(char c, bool *in_single_quotes, long *char_cnt)
{
	++(*char_cnt);
	if (c == '\'')
		*in_single_quotes = !(*in_single_quotes);
}

long	identifier_count_new_string_len(char *str, t_env *env, bool heredoc_single_quotes)
{
	long	char_cnt;
	long	i;
	bool	in_single_quotes;
	long	result;

	if (!str)
		return (0);
	in_single_quotes = false;
	i = 0;
	char_cnt = 0;
	while (str[i])
	{
		if(str[i] == '$' && (!in_single_quotes || heredoc_single_quotes))
		{
			result = handle_env_var_expansion(str, &i, &char_cnt, env);
			if (result < 0)
				return (result);
		}
		else
			handle_regular_char(str[i], &in_single_quotes, &char_cnt);
		++i;
	}
	return (char_cnt);
}

// We have error_flag here because functions can return NULL
// as valid value when returning the value for the
// environment variable that is not set
char	*get_env_var_value(char *str, long var_name_len, bool *error_flag, t_env *env)
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

static void	copy_regular_char(char c, char *new_str, long *new_cnt,
		bool *in_single_quotes)
{
	new_str[*new_cnt] = c;
	++(*new_cnt);
	if (c == '\'')
		*in_single_quotes = !(*in_single_quotes);
}

void	copy_var_to_new_str(char *str, char *var_value, long *new_cnt)
{
	long	var_value_len;

	var_value_len = ft_strlen(var_value);
	if (var_value)
	{
		ft_memcpy(str, var_value, var_value_len);
		(*new_cnt) += var_value_len;
	}
}

// else part is when we have bad or nonexistant variable naming,
// then we should just copy only the $ sign
static bool	copy_expanded_var(char *str, char *new_str, long *i, long *new_cnt, t_env *env)
{
	long	var_name_len;
	char	*var_value;
	bool	error_flag;

	error_flag = false;
	var_name_len = get_env_var_name_len(&str[*i + 1]);
	if (var_name_len > 0)
	{
		++(*i);
		var_value = get_env_var_value(&str[*i], var_name_len, &error_flag, env);
		if (error_flag)
			return (false);
		copy_var_to_new_str(&new_str[*new_cnt], var_value, new_cnt);
		*i += (var_name_len - 1);
	}
	else
		new_str[(*new_cnt)++] = str[*i];
	return (true);
}

static void copy_old_exit_value(int old_exit_value, char *new_str, long *new_cnt, long *i)
{
	int		old_exit_value_num_of_digits;
	int		old_exit_value_num_of_digits_copy;
	
	++(*i); 
	old_exit_value_num_of_digits = count_digits(old_exit_value);
	old_exit_value_num_of_digits_copy = old_exit_value_num_of_digits;
	--old_exit_value_num_of_digits;
	while(old_exit_value_num_of_digits >= 0)
	{
		new_str[*new_cnt + old_exit_value_num_of_digits] = '0' + old_exit_value % 10;
		old_exit_value = old_exit_value / 10;
		--old_exit_value_num_of_digits;
	}
	*new_cnt += old_exit_value_num_of_digits_copy;
}

bool	identifier_expand_var_to_string(char *str, char *new_str, t_env *env, bool heredoc_single_quotes)
{
	bool	in_single_quotes;
	long	i;
	long	new_cnt;

	in_single_quotes = false;
	i = 0;
	new_cnt = 0;
	while (str[i])
	{
		if(str[i] == '$' && (!in_single_quotes || heredoc_single_quotes))
		{
			if(str[i + 1] && str[i+1] == '?')
				copy_old_exit_value(env->old_exit_value, new_str, &new_cnt, &i);
			else if (!copy_expanded_var(str, new_str, &i, &new_cnt, env))
				return (false);
		}
		else
			copy_regular_char(str[i], new_str, &new_cnt, &in_single_quotes);
		++i;
	}
	if (in_single_quotes)
		return (false);
	return (true);
}

char	*identifier_expand_environment_variables(char *str, long new_size, t_env *env, bool heredoc_single_quotes)
{
	char	*new_string;

	new_string = (char *)malloc((new_size + 1));
	if (!new_string)
		return (NULL);
	new_string[new_size] = '\0';
	if (!identifier_expand_var_to_string(str, new_string, env, heredoc_single_quotes))
		return (free(new_string), NULL);
	return (new_string);
}

bool	is_containing_quotes(char *str)
{
	long	cnt;

	cnt = 0;
	while(str[cnt])
	{
		if(str[cnt] == '\"' || str[cnt] == '\'')
			return true;
		++cnt;
	}
	return false;
}

/// @brief Returns NULL if the allocation failed; do not free input string
/// @param str do not free this, because they are part of ast
/// @return
char	*identifier_expand_variables_in_string(char *str, t_env *env, bool heredoc_single_quotes)
{
	long	new_size;
	char	*new_str;

	if (!str)
		return (NULL);
	new_size = identifier_count_new_string_len(str, env, heredoc_single_quotes);
	// if(new_size == 0)
	// 	return (*empty_str = true, NULL);
	if (new_size < 0)
		return (NULL);
	new_str = identifier_expand_environment_variables(str, new_size, env, heredoc_single_quotes);
	if (!new_str)
		return (NULL);
	return (new_str);
}
