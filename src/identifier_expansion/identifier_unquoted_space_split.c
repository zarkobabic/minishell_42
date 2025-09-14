/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   identifier_unquoted_space_split.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 23:18:51 by zbabic            #+#    #+#             */
/*   Updated: 2025/09/14 12:28:09 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "identifier.h"
#include <stdbool.h>
#include <stdlib.h>

void	free_split(char **split)
{
	long	i;

	if (!split)
		return ;
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

static char	*extract_word(const char *str, long start, long len)
{
	char	*word;
	long	i;
	long	j;

	word = malloc(sizeof(char) * (len + 1));
	if (!word)
		return (NULL);
	i = start;
	j = 0;
	while (j < len)
	{
		word[j] = str[i];
		i++;
		j++;
	}
	word[j] = '\0';
	return (word);
}

static bool	fill_result_array(char **result, const char *str, long word_count)
{
	long	i;
	long	word_idx;
	long	start;
	long	len;

	i = 0;
	word_idx = 0;
	while (str[i] && word_idx < word_count)
	{
		while (str[i] == ' ')
			++i;
		start = i;
		len = get_word_len(str, &i);
		result[word_idx] = extract_word(str, start, len);
		if (!result[word_idx])
			return (false);
		word_idx++;
	}
	result[word_idx] = NULL;
	return (true);
}

char	**split_by_unquoted_space(const char *str)
{
	char	**result;
	long	word_count;

	if (!str)
		return (NULL);
	word_count = count_words(str);
	result = malloc(sizeof(char *) * (word_count + 1));
	if (!result)
		return (NULL);
	if (!fill_result_array(result, str, word_count))
	{
		free_split(result);
		return (NULL);
	}
	return (result);
}
