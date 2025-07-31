/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tree_parse_commands.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 04:02:57 by zbabic            #+#    #+#             */
/*   Updated: 2025/07/31 04:12:02 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

// TODO: dodaj parsiranje stringa kako treba, sa sve
// proverom za komandu ako je prvi put, expandovanje variabli etc.
// - OVO TAKODJE URADITI U AST!!!
//  command_element ::= TOKEN_IDENTIFIER | redirection
t_command_element	*parse_command_element(t_lexer *lexer)
{
	t_token				*token;
	char				*value;
	t_redirection		*redirection;
	t_command_element	*res_element;

	token = lexer_peek_next_token(lexer);
	if (!token)
		return (NULL);
	if (token->type == TOKEN_IDENTIFIER)
	{
		value = token->value;
		lexer_consume_and_free_next_token(lexer);
		return (create_identifier_element(value));
	}
	else if (is_redirection_token(token->type))
	{
		redirection = parse_redirection(lexer);
		if (!redirection)
			return (NULL);
		res_element = create_redirection_element(redirection);
		if (!res_element)
			return (free_redirection(redirection), NULL);
		return (res_element);
	}
	return (NULL);
}

// simple_command ::= command_element simple_command | command_element
t_simple_command	*parse_simple_command(t_lexer *lexer)
{
	t_command_element	*element;
	t_token				*token;
	t_simple_command	*next_simple_command;
	t_simple_command	*curr_command;

	element = parse_command_element(lexer);
	if (!element)
		return (NULL);
	token = lexer_peek_next_token(lexer);
	if (token && (token->type == TOKEN_IDENTIFIER
			|| is_redirection_token(token->type)))
	{
		next_simple_command = parse_simple_command(lexer);
		if (!next_simple_command)
			return (free_command_element(element), NULL);
		curr_command = create_simple_command(element, next_simple_command);
		if (!curr_command)
			return (free_command_element(element),
				free_simple_command(next_simple_command), NULL);
		return (curr_command);
	}
	curr_command = create_simple_command(element, NULL);
	if (!curr_command)
		return (free_command_element(element), NULL);
	return (curr_command);
}

// command ::= TOKEN_LPAR logical_expr TOKEN_RPAR redirection_list
static t_command	*parse_composite_command(t_lexer *lexer)
{
	t_token				*token;
	t_logical_expr		*logical_expr;
	t_redirection_list	*redirection_list;
	t_command			*new_cmd_node;
	bool				error;

	lexer_consume_and_free_next_token(lexer);
	logical_expr = parse_logical_expr(lexer);
	if (!logical_expr)
		return (perror(ERROR_SUBSHELL_EXPECTS_COMMAND), NULL);
	token = lexer_peek_next_token(lexer);
	if (!token || token->type != TOKEN_RPAR)
		return (perror(ERROR_EXPRECTED_RIGHT_PARENTHESIS),
			free_logical_expr(logical_expr), NULL);
	lexer_consume_and_free_next_token(lexer);
	error = false;
	redirection_list = parse_redirection_list(lexer, &error);
	if (error)
		return (free_logical_expr(logical_expr),
			free_redirection_list(redirection_list), NULL);
	new_cmd_node = create_composite_cmd(logical_expr, redirection_list);
	if (!new_cmd_node)
		return (free_logical_expr(logical_expr),
			free_redirection_list(redirection_list), NULL);
	return (new_cmd_node);
}

// command ::= simple_command 
//           | TOKEN_LPAR logical_expr TOKEN_RPAR redirection_list
t_command	*parse_command(t_lexer *lexer)
{
	t_token				*token;
	t_simple_command	*simple_command;
	t_command			*new_command;

	token = lexer_peek_next_token(lexer);
	if (!token)
		return (NULL);
	if (token->type == TOKEN_LPAR)
		return (parse_composite_command(lexer));
	else
	{
		simple_command = parse_simple_command(lexer);
		if (!simple_command)
			return (NULL);
		new_command = create_simple_command_node(simple_command);
		if (!new_command)
			return (free_simple_command(simple_command), NULL);
		return (new_command);
	}
}
