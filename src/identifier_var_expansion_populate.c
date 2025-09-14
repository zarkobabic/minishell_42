/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   identifier_var_expansion_populate.c                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 13:12:38 by zbabic            #+#    #+#             */
/*   Updated: 2025/09/14 14:00:19 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "exe_environment.h"
#include "identifier.h"
#include <stdbool.h>
#include <stdlib.h>

static void	copy_var_to_new_str(char *str, char *var_value, long *new_cnt)
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
static bool	copy_expanded_var(char **strings, long *i, long *new_cnt,
	t_env *env)
{
	long	var_name_len;
	char	*var_value;
	bool	error_flag;

	error_flag = false;
	var_name_len = get_env_var_name_len(&strings[ORIGINAL][*i + 1]);
	if (var_name_len > 0)
	{
		++(*i);
		var_value = get_env_var_value(&strings[ORIGINAL][*i], var_name_len,
				&error_flag, env);
		if (error_flag)
			return (false);
		copy_var_to_new_str(&strings[NEW][*new_cnt], var_value, new_cnt);
		*i += (var_name_len - 1);
	}
	else
		strings[NEW][(*new_cnt)++] = strings[ORIGINAL][*i];
	return (true);
}

static void	copy_old_exit_value(int old_exit_value, char *new_str,
	long *new_cnt, long *i)
{
	int	old_exit_value_num_of_digits;
	int	old_exit_value_num_of_digits_copy;

	++(*i);
	old_exit_value_num_of_digits = count_digits(old_exit_value);
	old_exit_value_num_of_digits_copy = old_exit_value_num_of_digits;
	--old_exit_value_num_of_digits;
	while (old_exit_value_num_of_digits >= 0)
	{
		new_str[*new_cnt + old_exit_value_num_of_digits] = '0' + old_exit_value
			% 10;
		old_exit_value = old_exit_value / 10;
		--old_exit_value_num_of_digits;
	}
	*new_cnt += old_exit_value_num_of_digits_copy;
}

static bool	identifier_expand_var_to_string(char **strs, t_env *env,
		bool heredoc_single_quotes)
{
	bool	in_quotes[2];
	long	i;
	long	new_cnt;

	in_quotes[SINGLE_QUOTES] = false;
	in_quotes[DOUBLE_QUOTES] = false;
	i = 0;
	new_cnt = 0;
	while (strs[ORIGINAL][i])
	{
		if (strs[ORIGINAL][i] == '$' && (!in_quotes[SINGLE_QUOTES]
				|| heredoc_single_quotes))
		{
			if (strs[ORIGINAL][i + 1] && strs[ORIGINAL][i + 1] == '?')
				copy_old_exit_value(env->old_exit_value, strs[0], &new_cnt, &i);
			else if (!copy_expanded_var(strs, &i, &new_cnt, env))
				return (false);
		}
		else
			copy_regular_char(strs[1][i], strs[NEW], &new_cnt, in_quotes);
		++i;
	}
	if (in_quotes[SINGLE_QUOTES] || in_quotes[DOUBLE_QUOTES])
		return (false);
	return (true);
}

char	*identifier_expand_environment_variables(char *str, long new_size,
	t_env *env, bool heredoc_single_quotes)
{
	char	*strings[2];

	strings[NEW] = (char *)malloc((new_size + 1));
	strings[ORIGINAL] = str;
	if (!strings[NEW])
		return (NULL);
	strings[NEW][new_size] = '\0';
	if (!identifier_expand_var_to_string(strings, env, heredoc_single_quotes))
		return (free(strings[NEW]), NULL);
	return (strings[NEW]);
}
