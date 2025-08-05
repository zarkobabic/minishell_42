/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   identifier_unquoted_space_split.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 23:18:51 by zbabic            #+#    #+#             */
/*   Updated: 2025/08/06 00:51:54 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdlib.h>

void	handle_quote(const char *str, long *i, bool *in_word, long *count)
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

void	handle_quote_len(const char *str, long *i, long *len)
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

char	*extract_word(const char *str, long start, long len)
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

bool	fill_result_array(char **result, const char *str, long word_count)
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

// #include <stdio.h>

// int	main(void)
// {
// 	char	**result;
// 	long	i;
// 	char	*test = "hello.   $mama asd $$$ 'world test'
// \"another quote\" asdsad &&&& sadfas ^^ asdf $$.    ";

// 	result = split_by_unquoted_space(test);
// 	if (!result)
// 	{
// 		printf("Error: split_by_unquoted_space returned NULL\n");
// 		return (1);
// 	}
// 	i = 0;
// 	while (result[i])
// 	{
// 		printf("Word %ld: |%s|\n", i, result[i]);
// 		i++;
// 	}
// 	free_split(result);
// 	return (0);
// }
