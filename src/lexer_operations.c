/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_operations.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 01:55:02 by zbabic            #+#    #+#             */
/*   Updated: 2025/07/31 00:12:20 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

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
	return (token_create(type));
}

/// @brief Peek the next token and stores it until consumed
/// @param lexer Lexer to peek the next token from
/// @return FAIL: NULL if allocation for token failed, SUCCESS: peeked token
t_token	*lexer_peek_next_token(t_lexer *lexer)
{
	if (!lexer)
		return (NULL);
	if (lexer->has_peeked && lexer->peeked_token)
		return (lexer->peeked_token);
	lexer->peeked_token = lexer_get_next_token(lexer);
	if (!lexer->peeked_token)
		return (NULL);
	lexer->has_peeked = true;
	return (lexer->peeked_token);
}

/// @brief Consumes and returns next token from the input
/// @param lexer Lexer to consume token from
/// @return FAIL: NULL if token allocation failed, SUCCESS: next token
static t_token	*lexer_consume_next_token(t_lexer *lexer)
{
	t_token	*next_token;

	if (!lexer)
		return (NULL);
	if (lexer->has_peeked && lexer->peeked_token)
	{
		next_token = lexer->peeked_token;
		lexer->peeked_token = NULL;
		lexer->has_peeked = false;
		return (next_token);
	}
	return (lexer_get_next_token(lexer));
}

void	lexer_consume_and_free_next_token(t_lexer *lexer)
{
	t_token	*next_token;

	next_token = lexer_consume_next_token(lexer);
	token_free_shallow(&next_token);
}
