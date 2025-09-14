/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   identifier_unquoted_space_utils.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 12:25:01 by zbabic            #+#    #+#             */
/*   Updated: 2025/09/14 14:20:46 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>

static void	handle_quote(const char *str, long *i, bool *in_word, long *count)
{
	char	quote;

	quote = str[(*i)++];
	if (!*in_word)
	{
		(*count)++;
		*in_word = true;
	}
	while (str[*i] && str[*i] != quote)
		(*i)++;
	if (str[*i])
		(*i)++;
}

long	count_words(const char *str)
{
	long	count;
	long	i;
	bool	in_word;

	count = 0;
	i = 0;
	in_word = false;
	while (str[i])
	{
		if (str[i] == ' ')
		{
			in_word = false;
			i++;
		}
		else if (str[i] == '\'' || str[i] == '"')
			handle_quote(str, &i, &in_word, &count);
		else
		{
			if (!in_word && ++count)
				in_word = true;
			i++;
		}
	}
	return (count);
}

static void	handle_quote_len(const char *str, long *i, long *len)
{
	char	quote;

	quote = str[(*i)++];
	(*len)++;
	while (str[*i] && str[*i] != quote)
	{
		(*len)++;
		(*i)++;
	}
	if (str[*i])
	{
		(*len)++;
		(*i)++;
	}
}

long	get_word_len(const char *str, long *i)
{
	long	len;

	len = 0;
	while (str[*i] && str[*i] != ' ')
	{
		if (str[*i] == '\'' || str[*i] == '"')
			handle_quote_len(str, i, &len);
		else
		{
			len++;
			(*i)++;
		}
	}
	return (len);
}
