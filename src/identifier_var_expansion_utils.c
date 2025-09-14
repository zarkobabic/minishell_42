/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   identifier_var_expansion_3.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 13:12:38 by zbabic            #+#    #+#             */
/*   Updated: 2025/09/14 13:41:16 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "exe_environment.h"
#include "identifier.h"
#include <stdbool.h>
#include <stdlib.h>

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

bool	is_containing_quotes(char *str)
{
	long	cnt;

	cnt = 0;
	while (str[cnt])
	{
		if (str[cnt] == '\"' || str[cnt] == '\'')
			return (true);
		++cnt;
	}
	return (false);
}

static bool	check_env_var_naming(char *str)
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
