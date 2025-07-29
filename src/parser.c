/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 17:32:25 by zbabic            #+#    #+#             */
/*   Updated: 2025/07/29 17:54:24 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "parser.h"
#define ERROR_LEXER_ALLOCATION_FAILED "Error: Allocation of the lexer failed"
#define ERROR_EXPECTED_INDENTIFIER_IN_REDIRECTION "Parsing error: Expected_indentifier in redirection"
#define ERROR_SUBSHELL_EXPECTS_COMMAND "Parsing error: Subshell call must have command to execute"
#define ERROR_EXPRECTED_RIGHT_PARENTHESIS "Parsing error: Exprected ')'"
#define ERROR_INVALID_SYNTAX "Parsing error: Syntax is not valid!"
#define ERROR_TREE_PARSING_FAILED "Parsing error: Parsing of the tree failed!"

bool	is_redirection_token(t_token_type type)
{
	return (type == TOKEN_REDIRECT_IN || type == TOKEN_REDIRECT_OUT
		|| type == TOKEN_REDIRECTION_HEREDOC || type == TOKEN_REDIRECT_APPEND);
}

// redirection ::= TOKEN_REDIRECT_IN TOKEN_IDENTIFIER | TOKEN_REDIRECT_OUT TOKEN_IDENTIFIER
//              | TOKEN_REDIRECT_APPEND TOKEN_IDENTIFIER | TOKEN_HEREDOC TOKEN_IDENTIFIER
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
		// TODO: proveriti ovde da li se taj identifier expanduje na vise od jedne reci (ima razmak koji nije pod navodnicima,
		// obicnim ili duplim,
		// i ako se razlaze vratiti redirection is ambiguous),
		// IPAK URADITI OVO TOKOM AST CREATION, NE SAD!!
		return (perror(ERROR_EXPECTED_INDENTIFIER_IN_REDIRECTION), NULL);
	next_token_value = token->value;
	lexer_consume_and_free_next_token(lexer);
	return (create_redirection(next_token_red_type, next_token_value));
}

// redirection_list ::= redirection redirection_list | ε
t_redirection_list	*parse_redirection_list(t_lexer *lexer, bool *error_flag)
{
	t_token				*token;
	t_redirection		*redirection;
	t_redirection_list	*next;
	t_redirection_list	*list;

	token = lexer_peek_next_token(lexer);
	if (!token || !is_redirection_token(token->type))
		return (NULL); // ε (epsilon)
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

// TODO: dodaj parsiranje stringa kako treba, sa sve
// proverom za komandu ako je prvi put, expandovanje variabli etc.
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

// command ::= simple_command | TOKEN_LPAR logical_expr TOKEN_RPAR redirection_list
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

// pipeline ::= command TOKEN_PIPE pipeline | command
t_pipeline	*parse_pipeline(t_lexer *lexer)
{
	t_command	*command;
	t_token		*token;
	t_pipeline	*next_pipeline;
	t_pipeline	*new_pipeline_node;

	command = parse_command(lexer);
	if (!command)
		return (NULL);
	token = lexer_peek_next_token(lexer);
	if (token && token->type == TOKEN_PIPE)
	{
		lexer_consume_and_free_next_token(lexer);
		next_pipeline = parse_pipeline(lexer);
		if (!next_pipeline)
			return (free_command(command), NULL);
		new_pipeline_node = create_pipeline(command, next_pipeline);
		if (!new_pipeline_node)
			return (free_command(command), free_pipeline(next_pipeline), NULL);
		return (new_pipeline_node);
	}
	new_pipeline_node = create_pipeline(command, NULL);
	if (!new_pipeline_node)
		return (free_command(command), NULL);
	return (new_pipeline_node);
}

// next_logical_expr ::= (TOKEN_AND | TOKEN_OR) pipeline next_logical_expr | ε
t_next_logical_expr	*parse_next_logical_expr(t_lexer *lexer, bool *error_flag)
{
	t_token				*token;
	int					op_type;
	t_pipeline			*pipeline;
	t_next_logical_expr	*next;
	t_next_logical_expr	*new_expr_node;

	token = lexer_peek_next_token(lexer);
	if (!token || (token->type != TOKEN_AND && token->type != TOKEN_OR))
		return (NULL); // ε (epsilon), valid case
	op_type = token->type;
	lexer_consume_and_free_next_token(lexer);
	pipeline = parse_pipeline(lexer);
	if (!pipeline)
		return (*error_flag = true, NULL);
	next = parse_next_logical_expr(lexer, error_flag);
	if (*error_flag)
		return (free_pipeline(pipeline), free_next_logical_expr(next), NULL);
	new_expr_node = create_next_logical_expr(op_type, pipeline, next);
	if (!new_expr_node)
		return (*error_flag = true, free_pipeline(pipeline),
			free_next_logical_expr(next), NULL);
	return (new_expr_node);
}

// logical_expr ::= pipeline next_logical_expr
t_logical_expr	*parse_logical_expr(t_lexer *lexer)
{
	t_pipeline			*pipeline;
	t_next_logical_expr	*next;
	t_logical_expr		*new_logical_expr;
	bool				next_logical_expr_error_flag;

	pipeline = parse_pipeline(lexer);
	if (!pipeline)
		return (NULL);
	next_logical_expr_error_flag = false;
	next = parse_next_logical_expr(lexer, &next_logical_expr_error_flag);
	if (next_logical_expr_error_flag)
		return (free_pipeline(pipeline), free_next_logical_expr(next), NULL);
	new_logical_expr = create_logical_expr(pipeline, next);
	if (!new_logical_expr)
		return (free_pipeline(pipeline), free_next_logical_expr(next), NULL);
	return (new_logical_expr);
}

// command_line ::= logical_expr TOKEN_EOF | TOKEN_EOF
t_command_line	*parse_command_line(t_lexer *lexer)
{
	t_token			*token;
	t_logical_expr	*logical_expr;
	t_command_line	*new_command_line;

	token = lexer_peek_next_token(lexer);
	if (!token)
		return (NULL);
	if (token->type == TOKEN_EOF)
	{
		lexer_consume_and_free_next_token(lexer);
		return (create_command_line(NULL));
	}
	logical_expr = parse_logical_expr(lexer);
	if (!logical_expr)
		return (NULL);
	token = lexer_peek_next_token(lexer);
	if (!token || token->type != TOKEN_EOF)
		return (perror(ERROR_INVALID_SYNTAX), free_logical_expr(logical_expr),
			NULL);
	lexer_consume_and_free_next_token(lexer);
	new_command_line = create_command_line(logical_expr);
	if (!new_command_line)
		return (free_logical_expr(logical_expr), NULL);
	return (new_command_line);
}

t_command_line	*parse_tree(char *input)
{
	t_command_line	*root;
	t_lexer			*lexer;

	lexer = lexer_create();
	if (!lexer)
		return (perror(ERROR_LEXER_ALLOCATION_FAILED), NULL);
	lexer_init(lexer, input);
	root = parse_command_line(lexer);
	lexer_destroy(&lexer);
	if (!root)
		return (perror(ERROR_TREE_PARSING_FAILED), NULL);
	return (root);
}
