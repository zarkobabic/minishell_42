/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   identifier_var_expansion_entry_resolution.c        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 13:12:38 by zbabic            #+#    #+#             */
/*   Updated: 2025/09/14 13:59:52 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "exe_environment.h"
#include "identifier.h"
#include <stdbool.h>
#include <stdlib.h>

static long	handle_env_var_expansion(char *str, long *i, long *char_cnt,
	t_env *env)
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

static void	handle_regular_char(char c, bool *in_quotes, long *char_cnt)
{
	++(*char_cnt);
	if (c == '\'' && !in_quotes[DOUBLE_QUOTES])
		in_quotes[SINGLE_QUOTES] = !in_quotes[SINGLE_QUOTES];
	else if (c == '\"' && !in_quotes[SINGLE_QUOTES])
		in_quotes[DOUBLE_QUOTES] = !in_quotes[DOUBLE_QUOTES];
}

long	identifier_count_new_string_len(char *str, t_env *env,
		bool heredoc_single_quotes)
{
	long	char_cnt;
	long	i;
	bool	in_quotes[2];
	long	result;

	if (!str)
		return (0);
	in_quotes[SINGLE_QUOTES] = false;
	in_quotes[DOUBLE_QUOTES] = false;
	i = 0;
	char_cnt = 0;
	while (str[i])
	{
		if (str[i] == '$' && (!in_quotes[SINGLE_QUOTES]
				|| heredoc_single_quotes))
		{
			result = handle_env_var_expansion(str, &i, &char_cnt, env);
			if (result < 0)
				return (result);
		}
		else
			handle_regular_char(str[i], in_quotes, &char_cnt);
		++i;
	}
	return (char_cnt);
}
