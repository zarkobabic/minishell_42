/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 03:46:00 by zbabic            #+#    #+#             */
/*   Updated: 2025/07/31 03:46:17 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include <stdio.h>

static void	token_print_type_to_string(t_token_type type, char *value)
{
	char	*type_to_string;

	if (type == TOKEN_UNDEFINED)
		type_to_string = "TOKEN_UNDEFINED";
	else if (type == TOKEN_AND)
		type_to_string = "TOKEN_AND (&&)";
	else if (type == TOKEN_OR)
		type_to_string = "TOKEN_OR (||)";
	else if (type == TOKEN_REDIRECT_IN)
		type_to_string = "TOKEN_REDIRECT_IN (<)";
	else if (type == TOKEN_REDIRECT_OUT)
		type_to_string = "TOKEN_REDIRECT_OUT(>)";
	else if (type == TOKEN_REDIRECTION_HEREDOC)
		type_to_string = "TOKEN_REDIRECTION_HEREDOC(<<)";
	else if (type == TOKEN_REDIRECT_APPEND)
		type_to_string = "TOKEN_REDIRECT_APPEND(>>)";
	else if (type == TOKEN_PIPE)
		type_to_string = "TOKEN_PIPE(|)";
	else if (type == TOKEN_LPAR)
		type_to_string = "TOKEN_LPAR( ( )";
	else if (type == TOKEN_RPAR)
		type_to_string = "TOKEN_RPAR ( ) )";
	else
		type_to_string = "TOKEN_EOF";
	printf("TOKEN: type: %s, value: %s\n", type_to_string, value);
}

void	token_print(t_token *token)
{
	if (token->type == TOKEN_IDENTIFIER)
		printf("TOKEN: type: IDENTIFIER, value: %s\n", token->value);
	else
		token_print_type_to_string(token->type, token->value);
}
