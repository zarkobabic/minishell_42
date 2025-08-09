/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   identifier.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 00:24:19 by zbabic            #+#    #+#             */
/*   Updated: 2025/08/09 01:55:03 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "identifier.h"
#include <stdio.h>
#include <stdlib.h>

int	main(void)
{
	char	*string;
	char	*expanded_string;
	char	**splitted_string;
	long	i;
	char	*temp;
	char	**temp_matched;
	long	j;
	long	k;

	string = "koka$USER\"life_in_germany$=Sucks$helo\" ast* $pars* \" $pars* \" asdnfcerf'dsf$HOME'. kl";
	expanded_string = identifier_expand_variables_in_string(string);
	splitted_string = split_by_unquoted_space(expanded_string);
	free(expanded_string);
	k = 1;
	i = 0;
	while (splitted_string[i])
	{
		j = 0;
		temp_matched = get_all_matched_names(splitted_string[i]);
		while(temp_matched[j])
		{
			temp = remove_quotes(temp_matched[j]);
			printf("Argument: %ld, value: %s\n", k, temp);
			++k;
			free(temp_matched[j]);
			free(temp);
			++j;
		}
		free(temp_matched);
		free(splitted_string[i]);
		++i;
	}
	free(splitted_string);
	return (0);
}
