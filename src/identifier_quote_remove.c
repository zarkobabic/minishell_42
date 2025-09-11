/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   identifier_quote_remove.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 00:12:43 by zbabic            #+#    #+#             */
/*   Updated: 2025/09/12 01:10:17 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "identifier.h"

void	update_in_quotes(bool *in_quotes, char c)
{
	if (c == '\'' && !in_quotes[DOUBLE_QUOTES])
		in_quotes[SINGLE_QUOTES]= !in_quotes[SINGLE_QUOTES];
	else if (c == '\"' && !in_quotes[SINGLE_QUOTES])
		in_quotes[DOUBLE_QUOTES] = !in_quotes[DOUBLE_QUOTES];
}

static int	count_non_quotes(char *str, bool *in_quotes)
{
	int	i;
	int	count;

	in_quotes[SINGLE_QUOTES] = false;
	in_quotes[DOUBLE_QUOTES] = false;
	i = 0;
	count = 0;
	while (str[i])
	{
		if (!((str[i] == '\'' && !in_quotes[DOUBLE_QUOTES]) || 
			(str[i] == '\"' && !in_quotes[SINGLE_QUOTES])))
			count++;
		update_in_quotes(in_quotes, str[i]);
		i++;
	}
	in_quotes[SINGLE_QUOTES] = false;
	in_quotes[DOUBLE_QUOTES] = false;
	return (count);
}

/// @brief removes quotes in a string
/// (! does not free str !)
/// @param str 
/// @return 
char	*remove_quotes(char *str)
{
	char	*result;
	int		i;
	int		j;
	int		len;
	bool	in_quotes[2];

	if (!str)
		return (NULL);
	len = count_non_quotes(str, in_quotes);
	result = (char *)malloc(sizeof(char) * (len + 1));
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (!((str[i] == '\'' && !in_quotes[DOUBLE_QUOTES]) || (str[i] == '\"' && !in_quotes[SINGLE_QUOTES])))
			result[j++] = str[i];
		update_in_quotes(in_quotes, str[i]);
		i++;
	}
	result[j] = '\0';
	return (result);
}
