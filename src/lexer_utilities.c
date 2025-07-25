/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utilities.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:21:50 by zbabic            #+#    #+#             */
/*   Updated: 2025/07/25 13:22:48 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

bool	ft_isspace(char c)
{
	return (c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r' || c == ' ');
}

bool	lexer_is_special_symbol(char c)
{
	return (c == '&' || c == '|' || c == '(' || c == ')' || c == '<' || c == '>');
}

/// @brief Lexer advance one character if the current character
/// is not '\0' or the next psotion is out of scope
/// @param lexer The lexer to advance
void	lexer_advance(t_lexer *lexer)
{
	if (!lexer || !(lexer->current_char))
		return ;
	if (lexer->pos + 1 > lexer->length)
	{
		lexer->current_char = '\0';
		return ;
	}
	lexer->pos += 1;
	lexer->current_char = lexer->source[lexer->pos];
}

/// @brief Shortcut function to advance with lexer_advance() 
/// function and then call token_create() with token type passed as argument
/// @param lexer lexer_to_advance
/// @param type token_type_to_create
/// @return FAIL: NULL if the allocation of the token failed,
/// SUCCESS: Newly created token
t_token	*lexer_advance_and_return_new_token(t_lexer *lexer, t_token_type type)
{
	lexer_advance(lexer);
	return token_create(type);
}