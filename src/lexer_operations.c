/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_operations.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 19:30:59 by zbabic            #+#    #+#             */
/*   Updated: 2025/07/12 01:08:51 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

void	lexer_skip_whitespaces(t_lexer *lexer)
{
	if (!lexer)
		return ;
	while (lexer->source[lexer->pos] == '\t'
		|| lexer->source[lexer->pos] == '\n'
		|| lexer->source[lexer->pos] == '\v'
		|| lexer->source[lexer->pos] == '\f'
		|| lexer->source[lexer->pos] == '\r'
		|| lexer->source[lexer->pos] == ' ')
		lexer_advance(lexer);
}

//TODO: maybe need to check if the string
// is not terminated with '\0' but probably not
//if yes that the first if, but from shell input it should be
//always terminated or NULL

/// @brief Lexer advance one character if the current character
/// is not '\0' or the next psotion is out of scope
/// @param lexer The lexer to advance
void	lexer_advance(t_lexer *lexer)
{
	if (!lexer || !(lexer->current_char))
		return ;
	if (lexer->pos + 1 > lexer->length)
	{
		lexer->current_char = '\0'; //TODO: Maybe overkill, dont need to check this probably
		return ;
	}
	lexer->pos += 1;
	lexer->current_char = lexer->source[lexer->pos];
}

static t_token	*lexer_handle_single_quote(t_lexer *lexer)
{
	lexer_advance(lexer);
	lexer->token_start_pos = lexer->pos;
	while(lexer->current_char && lexer->current_char != '\'')
		lexer_advance(lexer);
	if(!lexer->current_char)
		return NULL; //TODO: Error unclosed quote
	else if(lexer->current_char == '\'')
		return create_token(&lexer->source[lexer->token_start_pos], lexer->pos - lexer->token_start_pos, TOKEN_IDENTIFIER);
}

static t_token *lexer_handle_double_quote(t_lexer *lexer)
{
	if(state) //
}

//TODO: Check when you finish parsing have you left in some of the states for quotes or double quotes,
// because this is error in lexing
t_token *get_next_token(t_lexer *lexer)
{
	lexer_skip_whitespaces(lexer);
	if(lexer->current_char)
	{
		if(lexer->current_char == '\'')
			return lexer_handle_single_quote(lexer);
		else if(lexer->current_char == '\"')
			return lexer_handle_double_quote(lexer);
	}
	return create_token(&lexer->source[lexer->pos], 0, TOKEN_EOF);
}



// //TODO: Check when you finish parsing have you left in some of the states for quotes or double quotes,
// // because this is error in lexing
// t_token *get_next_token(t_lexer *lexer)
// {
// 	while(lexer->current_char)
// 	{
// 		if(lexer->quote_state == STATE_NO_QUOTE)
// 		{
// 			lexer_skip_whitespaces(lexer);
// 		}
// 		else if(lexer->quote_state == STATE_SINGLE_QUOTE)
// 		{

// 		}
// 		else if(lexer->quote_state == STATE_DOUBLE_QUOTE)
// 		{

// 		}
// 		lexer_advance(lexer);
// 	}
// 	return create_token(&lexer->source[lexer->pos], 0, TOKEN_EOF);
// }
