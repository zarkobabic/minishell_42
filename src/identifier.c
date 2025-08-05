/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   identifier.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 00:24:19 by zbabic            #+#    #+#             */
/*   Updated: 2025/08/06 00:52:35 by zbabic           ###   ########.fr       */
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

	string = "koka$USER\"life_in_germany$=Sucks$helo\"     $.    asdnfcerf'dsf$HOME'. kl";
	expanded_string = identifier_expand_variables_in_string(string);
	splitted_string = split_by_unquoted_space(expanded_string);
	free(expanded_string);
	i = 0;
	while (splitted_string[i])
	{
		temp = remove_quotes(splitted_string[i]);
		printf("Argument: %ld, value: %s\n", i + 1, temp);
		free(temp);
		free(splitted_string[i]);
		++i;
	}
	free(splitted_string);
	return (0);
}
