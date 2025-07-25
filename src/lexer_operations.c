/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_operations.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 19:30:59 by zbabic            #+#    #+#             */
/*   Updated: 2025/07/24 22:43:27 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#define ERROR_SINGLE_AMPERSAND "Syntax error: Operator & is not valid in the \
BASH syntax, did you maybe mean &&?"

bool	ft_isspace(char c)
{
	return (c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r' || c == ' ');
}

bool	lexer_is_special_symbol(char c)
{
	return (c == '&' || c == '|' || c == '(' || c == ')' || c == '<' || c == '>');
}

void	lexer_skip_whitespaces(t_lexer *lexer)
{
	if (!lexer)
		return ;
	while (ft_isspace(lexer->current_char))
		lexer_advance(lexer);
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

static t_token *lex_ampersands_and_vertical_bars(t_lexer *lexer)
{
	if(lexer->current_char == '&')
	{
		lexer_advance(lexer);
		if(lexer->current_char == '&')
			return (lexer_advance_and_return_new_token(lexer, TOKEN_AND));
		else
			return (perror(ERROR_SINGLE_AMPERSAND), token_create(TOKEN_UNDEFINED));
	}
	else if(lexer->current_char ==  '|')
	{
		lexer_advance(lexer);
		if(lexer->current_char == '|')
			return (lexer_advance_and_return_new_token(lexer, TOKEN_OR));
		else
			return (token_create(TOKEN_PIPE));
	}
	return token_create(TOKEN_UNDEFINED);
}

static t_token *lex_redirections(t_lexer *lexer)
{
	if(lexer->current_char == '<')
	{
		lexer_advance(lexer);
		if(lexer->current_char == '<')
			return (lexer_advance_and_return_new_token(lexer, TOKEN_REDIRECTION_HEREDOC));
		else
			return token_create(TOKEN_REDIRECT_IN);
	}
	else if(lexer->current_char == '>')
	{
		lexer_advance(lexer);
		if(lexer->current_char == '>')
			return (lexer_advance_and_return_new_token(lexer, TOKEN_REDIRECT_APPEND));
		else
			return (token_create(TOKEN_REDIRECT_OUT));
	}
	return token_create(TOKEN_UNDEFINED);
}

/// @brief Lex the identifier from the lexer input. 
/// Identifier is the sequence of different string elements that can be either of
/// t_component_type.
/// Lexing of the identifier finishes if we hit a whitespace or end of the input.
/// @param lexer lexer to use for lexing of the identifier 
/// @return NULL if the allocation failed, token_ident if it is successfully executed
static t_token	*lex_identifier(t_lexer *lexer)
{
	t_token	*token_ident;

	token_ident = token_create(TOKEN_IDENTIFIER);
	if(!token_ident)
		return NULL;
	lexer->token_start_pos = lexer->pos;
	while(lexer->current_char && !ft_isspace(lexer->current_char) && !lexer_is_special_symbol(lexer->current_char))
		lexer_advance(lexer);
	if(!token_add_value(token_ident, &lexer->source[lexer->token_start_pos], lexer->pos - lexer->token_start_pos))
		return NULL;
	return token_ident;
}

/// @brief Lexes next token using the lexer passed as argument
/// @param lexer lexer to use for lexing
/// @return SUCCESS: Next token that is lexed from the lexer input 
/// FAIL: NULL if the allocation for some token failed, 
/// TOKEN_UNDEFINED if the syntax is wrong
t_token *get_next_token(t_lexer *lexer)
{
	lexer_skip_whitespaces(lexer);
	while(lexer->current_char)
	{
		if(lexer->current_char == '&' || lexer->current_char ==  '|')
			return lex_ampersands_and_vertical_bars(lexer);
		else if(lexer->current_char == '(')
			return (lexer_advance_and_return_new_token(lexer, TOKEN_LPAR));
		else if(lexer->current_char == ')')
			return (lexer_advance_and_return_new_token(lexer, TOKEN_RPAR));
		else if(lexer->current_char == '<' || lexer->current_char == '>')
			return lex_redirections(lexer);
		else
			return lex_identifier(lexer);
	}
	return token_create(TOKEN_EOF);
}
