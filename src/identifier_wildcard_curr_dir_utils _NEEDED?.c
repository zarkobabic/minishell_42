/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   identifier_wildcard_curr_dir_utils _NEEDED?        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 12:32:56 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/14 12:41:22 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// FIXME: these functions are not used anywhere, delete?

// I have found on the internet that BASH uses normal
// alphabetical sort just with a tweak that
// '_' (ASCII 95) is treated as less than '.' (ASCII 46)
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
