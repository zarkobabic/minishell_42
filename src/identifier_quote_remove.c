/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   identifier_quote_remove.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 00:12:43 by zbabic            #+#    #+#             */
/*   Updated: 2025/09/06 02:09:34 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static int	count_non_quotes(char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] != '\'' && str[i] != '"')
			count++;
		i++;
	}
	return (count);
}

/// @brief removes quotes in a string; frees input string and returns modified string
/// @param str 
/// @return 
char	*remove_quotes(char *str)
{
	char	*result;
	int		i;
	int		j;
	int		len;

	if (!str)
		return (NULL);
	len = count_non_quotes(str);
	result = (char *)malloc(sizeof(char) * (len + 1));
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] != '\'' && str[i] != '"')
		{
			result[j] = str[i];
			j++;
		}
		i++;
	}
	result[j] = '\0';
	free(str);
	return (result);
}
