/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 17:32:25 by zbabic            #+#    #+#             */
/*   Updated: 2025/07/26 17:22:55 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "parser.h"
#define ERROR_LEXER_ALLOCATION_FAILED "Error: Allocation of the lexer failed"
#define ERROR_IN_PARSING "Parser error: Lexing error stopped the execution of the parser"

// void	parse_input(char *input)
// {
// 	t_lexer *lexer;
// 	t_token *token;

// 	lexer = lexer_create();
// 	if(!lexer)
// 		return (perror(ERROR_LEXER_ALLOCATION_FAILED));
// 	lexer_init(lexer, input);
// 	token = lexer_consume_next_token(lexer);
// 	while(token->type != TOKEN_EOF && token->type != TOKEN_UNDEFINED)
// 	{
// 		token_print(token);
// 		token_free(&token);
// 		t_token *temp = lexer_peek_next_token(lexer);
// 		printf("peeked\n");
// 		token_print(temp);
// 		printf("end_peeked\n");
// 		token = lexer_consume_next_token(lexer);
// 	}
// 	lexer_destroy(&lexer);
// 	if(token->type == TOKEN_UNDEFINED)
// 		return (perror(ERROR_IN_PARSING));
// 	token_print(token);
// 	token_free(&token);
// }

bool	is_redirection_token(t_token_type type)
{
	return (type == TOKEN_REDIRECT_IN || type == TOKEN_REDIRECT_OUT
		|| type == TOKEN_REDIRECTION_HEREDOC || type == TOKEN_REDIRECT_APPEND);
}

// redirection ::= TOKEN_REDIRECT_IN TOKEN_IDENTIFIER | TOKEN_REDIRECT_OUT TOKEN_IDENTIFIER
//              | TOKEN_REDIRECT_APPEND TOKEN_IDENTIFIER | TOKEN_HEREDOC TOKEN_IDENTIFIER
t_redirection	*parse_redirection(t_lexer *lexer)
{
	t_token			*token;
	int	next_token_red_type;
	char			*next_token_value;

	token = lexer_peek_next_token(lexer);
	if (!token || !is_redirection_token(token->type))
		return (NULL);
	next_token_red_type = token->type;
	lexer_consume_and_free_next_token(lexer);
	token = lexer_peek_next_token(lexer);
	if (!token || token->type != TOKEN_IDENTIFIER)
		return (perror("Parsing error: Expected_indentifier in redirection"),
			NULL);
	token = lexer_consume_next_token(lexer);
	next_token_value = token->value;
	token_free_shallow(&token);
	return (create_redirection(next_token_red_type, next_token_value));
}

// redirection_list ::= redirection redirection_list | ε
t_redirection_list	*parse_redirection_list(t_lexer *lexer)
{
	t_token				*token;
	t_redirection		*redirection;
	t_redirection_list	*next;

	token = lexer_peek_next_token(lexer);
	if (!token || !is_redirection_token(token->type))
		return (NULL); // ε (epsilon)
	redirection = parse_redirection(lexer);
	if (!redirection)
		return (NULL);
	next = parse_redirection_list(lexer);
	return (create_redirection_list(redirection, next));
}

// TODO: dodaj parsiranje stringa kako treba, sa sve
// proverom za komandu ako je prvi put, expandovanje variabli etc.
//  command_element ::= TOKEN_IDENTIFIER | redirection
t_command_element	*parse_command_element(t_lexer *lexer)
{
	t_token			*token;
	char			*value;
	t_redirection	*redirection;

	token = lexer_peek_next_token(lexer);
	if (!token)
		return (NULL);
	if (token->type == TOKEN_IDENTIFIER)
	{
		token = lexer_consume_next_token(lexer);
		value = token->value;
		token_free_shallow(&token);
		return (create_identifier_element(value));
	}
	else if (is_redirection_token(token->type))
	{
		redirection = parse_redirection(lexer);
		if (!redirection)
			return (NULL);
		return (create_redirection_element(redirection));
	}
	return (NULL);
}

// simple_command ::= command_element simple_command | command_element
t_simple_command	*parse_simple_command(t_lexer *lexer)
{
	t_command_element	*element;
	t_token				*token;
	t_simple_command	*next_simple_command;

	element = parse_command_element(lexer);
	if (!element)
		return (NULL);
	token = lexer_peek_next_token(lexer);
	if (token && (token->type == TOKEN_IDENTIFIER
			|| is_redirection_token(token->type)))
	{
		next_simple_command = parse_simple_command(lexer);
        if (next_simple_command)
            return create_simple_command(element, next_simple_command);
	}
	return (create_simple_command(element, NULL));
}

// command ::= TOKEN_LPAR logical_expr TOKEN_RPAR redirection_list
static t_command	*parse_composite_command(t_lexer *lexer)
{
	t_token				*token;
	t_logical_expr		*logical_expr;
	t_redirection_list	*redirection_list;

	lexer_consume_and_free_next_token(lexer);
	logical_expr = parse_logical_expr(lexer);
	if (!logical_expr)
		return (perror("Parsing error: Subshell call must have command to execute"),
			NULL);
	token = lexer_peek_next_token(lexer);
	if (!token || token->type != TOKEN_RPAR)
		return (perror("Parsing error: Exprected ')'"), NULL);
	lexer_consume_and_free_next_token(lexer);
	redirection_list = parse_redirection_list(lexer);
	return (create_composite_command_node(logical_expr, redirection_list));
}

// command ::= simple_command | TOKEN_LPAR logical_expr TOKEN_RPAR redirection_list
t_command	*parse_command(t_lexer *lexer)
{
	t_token				*token;
	t_simple_command	*simple_command;

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
		return (create_simple_command_node(simple_command));
	}
}

// pipeline ::= command TOKEN_PIPE pipeline | command
t_pipeline	*parse_pipeline(t_lexer *lexer)
{
	t_command	*command;
	t_token		*token;
	t_pipeline	*next_pipeline;

	command = parse_command(lexer);
	if (!command)
		return (NULL);
	token = lexer_peek_next_token(lexer);
	if (token && token->type == TOKEN_PIPE)
	{
		lexer_consume_and_free_next_token(lexer);
		next_pipeline = parse_pipeline(lexer);
		if (!next_pipeline)
			return (NULL);
		return (create_pipeline(command, next_pipeline));
	}
	return (create_pipeline(command, NULL));
}

// next_logical_expr ::= (TOKEN_AND | TOKEN_OR) pipeline next_logical_expr | ε
t_next_logical_expr	*parse_next_logical_expr(t_lexer *lexer)
{
	t_token				*token;
	int					op_type;
	t_pipeline			*pipeline;
	t_next_logical_expr	*next;

	token = lexer_peek_next_token(lexer);
	if (!token || (token->type != TOKEN_AND && token->type != TOKEN_OR))
		return (NULL); // ε (epsilon)
	op_type = token->type;
	lexer_consume_and_free_next_token(lexer);
	pipeline = parse_pipeline(lexer);
	if (!pipeline)
		return (NULL);
	next = parse_next_logical_expr(lexer);
	return (create_next_logical_expr(op_type, pipeline, next));
}

// logical_expr ::= pipeline next_logical_expr
t_logical_expr	*parse_logical_expr(t_lexer *lexer)
{
	t_pipeline			*pipeline;
	t_next_logical_expr	*next;

	pipeline = parse_pipeline(lexer);
	if (!pipeline)
		return (NULL);
	next = parse_next_logical_expr(lexer);
	return (create_logical_expr(pipeline, next));
}

// command_line ::= logical_expr TOKEN_EOF | TOKEN_EOF
t_command_line	*parse_command_line(t_lexer *lexer)
{
	t_token			*token;
	t_logical_expr	*logical_expr;

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
		return (perror("Parsing error: Syntax is not valid!"), NULL);
	lexer_consume_and_free_next_token(lexer);
	return (create_command_line(logical_expr));
}

t_command_line	*parse_tree(char *input)
{
	t_command_line	*root;
	t_lexer *lexer;

	lexer = lexer_create();
	if(!lexer)
		return (perror(ERROR_LEXER_ALLOCATION_FAILED), NULL);
	lexer_init(lexer, input);
	root = parse_command_line(lexer);
	lexer_destroy(&lexer);
	if(!root)
		return (perror("Parsing error: Parsing of the tree failed!"), NULL);
	return root;
}
