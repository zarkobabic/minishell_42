/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   identifier_wildcard.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 15:23:41 by zbabic            #+#    #+#             */
/*   Updated: 2025/09/14 14:22:27 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "identifier.h"

static void	backtrack_to_curr_star(long *i, long *j, long *match,
		long *curr_star_pos)
{
	*j = *curr_star_pos + 1;
	++(*match);
	*i = *match;
}

static void	advance_both(long *i, long *j)
{
	++(*i);
	++(*j);
}

static void	set_curr_star(long *i, long *j, long *match, long *curr_star_pos)
{
	*curr_star_pos = *j;
	*match = *i;
	++(*j);
}

bool	wildcard_check_if_matches(char *input, char *pattern)
{
	long	i;
	long	j;
	long	curr_star_pos;
	long	match;

	i = 0;
	j = 0;
	curr_star_pos = -1;
	match = 0;
	while (input && input[i])
	{
		if (pattern[j] && pattern[j] == input[i])
			advance_both(&i, &j);
		else if (pattern[j] && pattern[j] == '*')
			set_curr_star(&i, &j, &match, &curr_star_pos);
		else if (curr_star_pos != -1)
			backtrack_to_curr_star(&i, &j, &match, &curr_star_pos);
		else
			return (false);
	}
	while (pattern[j] && pattern[j] == '*')
		++j;
	return (pattern[j] == '\0');
}
