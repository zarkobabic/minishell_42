/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utilities.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:21:50 by zbabic            #+#    #+#             */
/*   Updated: 2025/07/31 03:49:20 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

bool	lexer_isspace(char c)
{
	return (c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r'
		|| c == ' ');
}

bool	lexer_is_special_symbol(char c)
{
	return (c == '&' || c == '|' || c == '(' || c == ')' || c == '<'
		|| c == '>');
}

void	lexer_skip_whitespaces(t_lexer *lexer)
{
	if (!lexer)
		return ;
	while (lexer_isspace(lexer->current_char))
		lexer_advance(lexer);
}
