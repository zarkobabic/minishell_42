/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 17:33:04 by zbabic            #+#    #+#             */
/*   Updated: 2025/07/29 15:45:12 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

// GRAMMAR USED BY IMPLEMENTED RECURSIVE DESCENT PARSER:
//
//   command_line     ::= logical_expr TOKEN_EOF
//                     |  TOKEN_EOF
//
//   logical_expr     ::= pipeline next_logical_expr

//   next_logical_expr ::= (TOKEN_AND | TOKEN_OR) pipeline next_logical_expr
//                      |  ε
//
//   pipeline         ::= command TOKEN_PIPE pipeline
//                     |  command
//
//   command          ::= simple_command
//                     |  TOKEN_LPAR logical_expr TOKEN_RPAR redirection_list
//
//   simple_command   ::= command_element simple_command
//                     |  command_element 
//
//   command_element  ::= TOKEN_IDENTIFIER
//                     |  redirection
//
//   redirection_list ::= redirection redirection_list
//                     |   ε
//
//   redirection      ::= TOKEN_REDIRECT_IN TOKEN_IDENTIFIER
//                     |  TOKEN_REDIRECT_OUT TOKEN_IDENTIFIER
//                     |  TOKEN_REDIRECT_APPEND TOKEN_IDENTIFIER
//                     |  TOKEN_HEREDOC TOKEN_IDENTIFIER

#include "parse_tree_structures.h"

t_command_line	*parse_tree(char *input);
bool	is_redirection_token(t_token_type type);
void	free_parse_tree(t_command_line *parse_tree);

//Parse tree nodes constructors

t_command_line		*create_command_line(t_logical_expr *logical_expr);
t_logical_expr		*create_logical_expr(t_pipeline *pipeline, t_next_logical_expr *next);
t_next_logical_expr	*create_next_logical_expr(t_logical_expr_type op_type, t_pipeline *pipeline, t_next_logical_expr *next);
t_pipeline			*create_pipeline(t_command *command, t_pipeline *next);
t_command			*create_simple_command_node(t_simple_command *simple_command);
t_command			*create_composite_command_node(t_logical_expr *logical_expr, t_redirection_list *redirection_list);
t_simple_command	*create_simple_command(t_command_element *element, t_simple_command *next);
t_command_element	*create_identifier_element(char *identifier);
t_command_element	*create_redirection_element(t_redirection *redirection);
t_redirection_list	*create_redirection_list(t_redirection *redirection, t_redirection_list *next);
t_redirection		*create_redirection(t_redirection_type type, char *file);

//Parse tree nodes destructors

void	free_command_line(t_command_line *cmd_line);
void	free_logical_expr(t_logical_expr *logical_expr);
void	free_next_logical_expr(t_next_logical_expr *next_logical_expr);
void	free_pipeline(t_pipeline *pipeline);
void	free_command(t_command *command);
void	free_simple_command(t_simple_command *simple_command);
void	free_composite_command(t_composite_command *composite_command);
void	free_command_element(t_command_element *command_element);
void	free_redirection_list(t_redirection_list *redirection_list);
void	free_redirection(t_redirection *redirection);

//Recursive descent parsing functions

t_command_line		*parse_command_line(t_lexer *lexer);
t_logical_expr		*parse_logical_expr(t_lexer *lexer);
t_next_logical_expr	*parse_next_logical_expr(t_lexer *lexer);
t_pipeline			*parse_pipeline(t_lexer *lexer);
t_command			*parse_command(t_lexer *lexer);
t_simple_command	*parse_simple_command(t_lexer *lexer);
t_command_element	*parse_command_element(t_lexer *lexer);
t_redirection_list	*parse_redirection_list(t_lexer *lexer);
t_redirection		*parse_redirection(t_lexer *lexer);

//Printing function just for testing

void	parse_tree_print(t_command_line *command_line);

#endif