/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tree_parse_expressions.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 04:00:17 by zbabic            #+#    #+#             */
/*   Updated: 2025/07/31 04:42:44 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

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
// ε (epsilon), valid case on line 58
t_next_logical_expr	*parse_next_logical_expr(t_lexer *lexer, bool *error_flag)
{
	t_token				*token;
	int					op_type;
	t_pipeline			*pipeline;
	t_next_logical_expr	*next;
	t_next_logical_expr	*new_expr_node;

	token = lexer_peek_next_token(lexer);
	if (!token)
		return (*error_flag = true, NULL);
	else if (token->type != TOKEN_AND && token->type != TOKEN_OR)
		return (NULL);
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
