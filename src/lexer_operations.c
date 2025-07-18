/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_operations.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 19:30:59 by zbabic            #+#    #+#             */
/*   Updated: 2025/07/18 20:37:48 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

bool	ft_isspace(char c)
{
	return (c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r' || c == ' ');
}

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

/// @brief Shortcut function to advance with lexer_advance() 
/// function and then call create_token() with token type passed as argument
/// @param lexer lexer_to_advance
/// @param type token_type_to_create
/// @return FAIL: NULL if the allocation of the token failed,
/// SUCCESS: Newly created token
t_token	*lexer_advance_and_return_new_token(t_lexer *lexer, t_token_type type)
{
	lexer_advance(lexer);
	return create_token(type);
}

//TODO: promeni ovo posto moras da napravis token pre kad prepoznas da je identifikator a "" ce samo da dodaju komponentu
// static bool	lexer_handle_single_quote(t_lexer *lexer, t_token *token_to_add_to)
// {
// 	lexer_advance(lexer);
// 	lexer->token_start_pos = lexer->pos;
// 	while(lexer->current_char && lexer->current_char != '\'')
// 		lexer_advance(lexer);
// 	if(!lexer->current_char)
// 		return false; //TODO: Error unclosed quote
// 	else if(lexer->current_char == '\'')
// 		token_add_value(token_to_add_to, &lexer->source[lexer->token_start_pos], lexer->pos - lexer->token_start_pos, COMPONENT_LITERAL);
// 	return true;
// }

// static t_token *lexer_handle_double_quote(t_lexer *lexer)
// {
// 	//TODO: Napisati ovu funkciju za double quotes da isparsira sve sto treba
// }

static t_token *lex_ampersands_and_vertical_bars(t_lexer *lexer)
{
	if(lexer->current_char == '&')
	{
		lexer_advance(lexer);
		if(lexer->current_char == '&')
			return (lexer_advance_and_return_new_token(lexer, TOKEN_AND));
		else
			return create_token(TOKEN_UNDEFINED); //cannot be only one &
	}
	else if(lexer->current_char ==  '|')
	{
		lexer_advance(lexer);
		if(lexer->current_char == '|')
			return (lexer_advance_and_return_new_token(lexer, TOKEN_OR));
		else
			return (create_token(TOKEN_PIPE));
	}
	return create_token(TOKEN_UNDEFINED);
}

static t_token *lex_redirections(t_lexer *lexer)
{
	if(lexer->current_char == '<')
	{
		lexer_advance(lexer);
		if(lexer->current_char == '<')
			return (lexer_advance_and_return_new_token(lexer, TOKEN_REDIRECTION_HEREDOC));
		else
			return create_token(TOKEN_REDIRECT_IN);
	}
	else if(lexer->current_char == '>')
	{
		lexer_advance(lexer);
		if(lexer->current_char == '>')
			return (lexer_advance_and_return_new_token(lexer, TOKEN_REDIRECT_APPEND));
		else
			return (create_token(TOKEN_REDIRECT_OUT));
	}
	return create_token(TOKEN_UNDEFINED);
}

/// @brief Adds the string sourounded by single quotes as a COMPONENT_LITERAL
/// type of string component to the composite token token_to_add_to
/// @param lexer lexer to lex input from
/// @param token_to_add_to composite token to add string component to
/// @retval FAIL: false if syntax is not good (unclosed single string)
/// or if allocation for string component fails
/// @retval SUCCESS: true if component is successfully added
bool	lex_single_quotes(t_lexer *lexer, t_token *token_to_add_to)
{
	lexer_advance(lexer);
	lexer->token_start_pos = lexer->pos;
	while(lexer->current_char && lexer->current_char != '\'')
		lexer_advance(lexer);
	if(!lexer->current_char)
		return false; //Error unclosed quote
	else if(lexer->current_char == '\'')
	{
		if(token_add_value(token_to_add_to, &lexer->source[lexer->token_start_pos], lexer->pos - lexer->token_start_pos, COMPONENT_LITERAL) == TOKEN_ADD_ERROR_VALUE_MALLOC_FAILED)
			return false;
		lexer_advance(lexer);
		return true;
	}
	return false;
}

//TODO: dodaj expanziju variabli
bool	lex_double_quotes(t_lexer *lexer, t_token *token_to_add_to)
{
	lexer_advance(lexer);
	lexer->token_start_pos = lexer->pos;
	while(lexer->current_char && lexer->current_char != '\"')
		lexer_advance(lexer);
	if(!lexer->current_char)
		return false; //Error unclosed quote
	else if(lexer->current_char == '\"')
	{
		if(token_add_value(token_to_add_to, &lexer->source[lexer->token_start_pos], lexer->pos - lexer->token_start_pos, COMPONENT_LITERAL) == TOKEN_ADD_ERROR_VALUE_MALLOC_FAILED)
			return false;
		lexer_advance(lexer);
		return true;
	}
	return false;
}

bool	lex_normal_string(t_lexer *lexer, t_token *token_to_add_to)
{
	t_component_type component_type;

	component_type = COMPONENT_LITERAL;
	lexer->token_start_pos = lexer->pos;
	while(lexer->current_char && !ft_isspace(lexer->current_char) && lexer->current_char != '\'' && lexer->current_char != '\"' && lexer->current_char != '$')
	{
		if(lexer->current_char == '*')
			component_type = COMPONENT_LITERAL_WILDCARD;
		lexer_advance(lexer);
	}
	if(token_add_value(token_to_add_to, &lexer->source[lexer->token_start_pos], lexer->pos - lexer->token_start_pos, component_type) == TOKEN_ADD_ERROR_VALUE_MALLOC_FAILED)
		return false;
	return true;
}

/// @brief Lex the identifier from the lexer input. 
/// Identifier is the sequence of different string elements that can be either of
/// t_component_type.
/// Lexing of the identifier finishes if we hit a whitespace or end of the input.
/// @param lexer lexer to use for lexing of the identifier 
/// @return 
t_token	*lex_identifier(t_lexer *lexer)
{
	t_token	*token_ident;
	bool	res;

	token_ident = create_token(TOKEN_IDENTIFIER);
	if(!token_ident)
		return NULL;
	while(lexer->current_char && !ft_isspace(lexer->current_char))
	{
		if(lexer->current_char == '\'')
			res = lex_single_quotes(lexer, token_ident);
		else if(lexer->current_char == '\"')
			res = lex_double_quotes(lexer, token_ident);
		else if(lexer->current_char == '$')
			res = false;
			// res = lex_variable(lexer, token_ident);
		else //lexiramo normalne reci, sve dok ne naidjemo na " ' $ whitespace ili kraj stringa
			res = lex_normal_string(lexer, token_ident);
		if(!res)
			return NULL; //Error some allocation failed, or syntax error
	}
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
		// lexer_skip_whitespaces(lexer); //TODO: proveri da li je ovo potrebno, skoro sigurno ne
	}
	return create_token(TOKEN_EOF);
}
