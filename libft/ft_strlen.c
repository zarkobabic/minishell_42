/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 17:41:44 by zbabic            #+#    #+#             */
/*   Updated: 2025/07/09 20:08:29 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlen(const char *s)
{
	int	counter;

	counter = 0;
	if (!s)
		return (0);
	while (s[counter] != '\0')
	{
		++counter;
	}
	return (counter);
}
// #include <stdio.h>
// int main(void)
// {
// 	printf("%zu",ft_strlen(""));
// 	return (0);
// }