/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tree_parse_redirections.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 17:32:25 by zbabic            #+#    #+#             */
/*   Updated: 2025/08/05 19:09:36 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

bool	is_redirection_token(t_token_type type)
{
	return (type == TOKEN_REDIRECT_IN || type == TOKEN_REDIRECT_OUT
		|| type == TOKEN_REDIRECTION_HEREDOC || type == TOKEN_REDIRECT_APPEND);
}

// redirection ::= TOKEN_REDIRECT_IN TOKEN_IDENTIFIER 
//              | TOKEN_REDIRECT_OUT TOKEN_IDENTIFIER
//              | TOKEN_REDIRECT_APPEND TOKEN_IDENTIFIER
//              | TOKEN_HEREDOC TOKEN_IDENTIFIER
t_redirection	*parse_redirection(t_lexer *lexer)
{
	t_token	*token;
	int		next_token_red_type;
	char	*next_token_value;

	token = lexer_peek_next_token(lexer);
	if (!token || !is_redirection_token(token->type))
		return (NULL);
	next_token_red_type = token->type;
	lexer_consume_and_free_next_token(lexer);
	token = lexer_peek_next_token(lexer);
	if (!token || token->type != TOKEN_IDENTIFIER)
		return (perror(ERROR_EXPECTED_INDENTIFIER_IN_REDIRECTION), NULL);
	next_token_value = token->value;
	lexer_consume_and_free_next_token(lexer);
	return (create_redirection(next_token_red_type, next_token_value));
}

// redirection_list ::= redirection redirection_list | ε
// Line 63, substitution with epsilon, // ε (epsilon), valid
t_redirection_list	*parse_redirection_list(t_lexer *lexer, bool *error_flag)
{
	t_token				*token;
	t_redirection		*redirection;
	t_redirection_list	*next;
	t_redirection_list	*list;

	token = lexer_peek_next_token(lexer);
	if (!token)
		return (*error_flag = true, NULL);
	else if (!is_redirection_token(token->type))
		return (NULL);
	redirection = parse_redirection(lexer);
	if (!redirection)
		return (*error_flag = true, NULL);
	next = parse_redirection_list(lexer, error_flag);
	if (*error_flag)
		return (free_redirection(redirection), free_redirection_list(next),
			NULL);
	list = create_redirection_list(redirection, next);
	if (!list)
		return (*error_flag = true, free_redirection(redirection),
			free_redirection_list(next), NULL);
	return (list);
}
