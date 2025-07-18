/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 17:32:25 by zbabic            #+#    #+#             */
/*   Updated: 2025/07/17 23:59:12 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "lexer.h"

void	parse_command(char *input)
{
	t_lexer *lexer;
	t_token *token;

	lexer = create_lexer();
	if(!lexer)
		return ; //TODO: Error lexer malloc failed
	init_lexer(lexer, input);
	token = get_next_token(lexer);
	while(token->type != TOKEN_EOF)
	{
		print_token(token);
		free_token(&token);
		token = get_next_token(lexer);
	}
	print_token(token);
	free_token(&token);
}
