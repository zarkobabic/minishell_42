/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   identifier_wildcard_curr_dir.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 16:40:40 by zbabic            #+#    #+#             */
/*   Updated: 2025/08/06 19:07:30 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "identifier.h"
#include <dirent.h>
#include <stdlib.h>

static void	string_array_free_all(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

static int	count_matches(DIR *dir, char *pattern)
{
	struct dirent	*entry;
	int				count;

	count = 0;
	entry = readdir(dir);
	while (entry)
	{
		if (entry->d_name[0] != '.' && wildcard_check_if_matches(entry->d_name,
				pattern))
			count++;
		entry = readdir(dir);
	}
	return (count);
}

static int	fill_matches(char **result, DIR *dir, char *pattern)
{
	struct dirent	*entry;
	int				i;
	long			size;

	i = 0;
	entry = readdir(dir);
	while (entry)
	{
		if (entry->d_name[0] != '.' && wildcard_check_if_matches(entry->d_name,
				pattern))
		{
			size = ft_strlen(entry->d_name);
			result[i] = malloc(size + 1);
			if (!result[i])
				return (string_array_free_all(result), -1);
			ft_memcpy(result[i], entry->d_name, size);
			result[i][size] = '\0';
			i++;
		}
		entry = readdir(dir);
	}
	result[i] = NULL;
	return (0);
}

// I have found on the internet that BASH uses normal
// alphabetical sort just with a tweak that
// _ (ASCII 95) is treated as less than . (ASCII 46)
int	bashcmp(char *s1, char *s2)
{
	unsigned char	c1;
	unsigned char	c2;

	while (*s1 && *s2)
	{
		c1 = (unsigned char)*s1;
		c2 = (unsigned char)*s2;
		if (c1 != c2)
		{
			if (c1 == '_' && c2 == '.')
				return (-1);
			if (c1 == '.' && c2 == '_')
				return (1);
			return ((int)c1 - (int)c2);
		}
		s1++;
		s2++;
	}
	return ((int)(unsigned char)*s1 - (int)(unsigned char)*s2);
}

void	bash_sort(char **array)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (array[i + 1])
	{
		j = 0;
		while (array[j + 1])
		{
			if (bashcmp(array[j], array[j + 1]) > 0)
			{
				tmp = array[j];
				array[j] = array[j + 1];
				array[j + 1] = tmp;
			}
			++j;
		}
		++i;
	}
}

/// @brief Creates an array of strings with the pattern as
/// a first element and then NULL. (This is implemented like this)
/// to keep the compactibility with the result if we have matches.
/// @param pattern Pattern to copy
/// @return FAIL: NULL if allocation failed
/// SUCCESS: NULL terminated array of strings
/// with original pattern as a first element
char	**copy_original_pattern(char *pattern)
{
	long	size;
	char	*pattern_copy;
	char	**res;

	res = (char **)malloc(sizeof(char *) * 2);
	if (!res)
		return (NULL);
	res[1] = NULL;
	size = ft_strlen(pattern);
	pattern_copy = malloc(size + 1);
	if (!pattern_copy)
		return (free(res), NULL);
	ft_memcpy(pattern_copy, pattern, size);
	pattern_copy[size] = '\0';
	res[0] = pattern_copy;
	return (res);
}

/// @brief Returns all the matched file or directory
/// names from the current directory
/// @param pattern
/// @return FAIL: NULL if allocation failed.
/// SUCCESS: NULL terminated array of strings that pattern
/// matched for successfull result
char	**get_all_matched_names(char *pattern)
{
	DIR		*dir;
	int		count;
	char	**result;

	dir = opendir(".");
	if (!dir)
		return (NULL);
	count = count_matches(dir, pattern);
	closedir(dir);
	if (count == 0)
		return (copy_original_pattern(pattern));
	result = malloc(sizeof(char *) * (count + 1));
	if (!result)
		return (NULL);
	dir = opendir(".");
	if (!dir)
		return (free(result), NULL);
	if (fill_matches(result, dir, pattern) == -1)
		return (closedir(dir), NULL);
	closedir(dir);
	return (result);
}

// #include <stdio.h>

// int	main(void)
// {
// 	char	**matched;
// 	long	i;

// 	i = 0;
// 	matched = get_all_matched_names("*qu*");
// 	bash_sort(matched);
// 	while (matched[i])
// 		printf("%s ", matched[i++]);
// 	string_array_free_all(matched);
// }
