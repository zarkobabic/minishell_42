/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 17:32:25 by zbabic            #+#    #+#             */
/*   Updated: 2025/07/13 00:46:58 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "lexer.h"

void	parse_command(char *input)
{
	t_lexer *lexer;

	lexer = create_lexer();
	if(!lexer)
		return ; //Error lexer malloc failed
	init_lexer(lexer, input);
	t_token *token = get_next_token(lexer);
	while(token != TOKEN_EOF)
	{
		//
	}
}
