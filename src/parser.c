/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 17:32:25 by zbabic            #+#    #+#             */
/*   Updated: 2025/07/25 14:26:32 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "lexer.h"
#define ERROR_LEXER_ALLOCATION_FAILED "Error: Allocation of the lexer failed"
#define ERROR_IN_PARSING "Parser error: Lexing error stopped the execution of the parser"

void	parse_input(char *input)
{
	t_lexer *lexer;
	t_token *token;

	lexer = lexer_create();
	if(!lexer)
		return perror(ERROR_LEXER_ALLOCATION_FAILED);
	lexer_init(lexer, input);
	token = lexer_get_next_token(lexer);
	while(token->type != TOKEN_EOF && token->type != TOKEN_UNDEFINED)
	{
		token_print(token);
		token_free(&token);
		token = lexer_get_next_token(lexer);
	}
	lexer_destroy(&lexer);
	if(token->type == TOKEN_UNDEFINED)
		return perror(ERROR_IN_PARSING);
	token_print(token);
	token_free(&token);
}
