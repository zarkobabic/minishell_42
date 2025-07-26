/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 17:33:04 by zbabic            #+#    #+#             */
/*   Updated: 2025/07/26 04:09:41 by zbabic           ###   ########.fr       */
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

void	parse_input(char *input);
bool	is_redirection_token(t_token_type type);

//Parse tree nodes constructors

t_command_line *create_command_line(t_logical_expr *logical_expr);
t_logical_expr *create_logical_expr(t_logical_expr_type op_type, t_pipeline *pipeline, t_next_logical_expr *next);
t_next_logical_expr *create_next_logical_expr(t_logical_expr_type op_type, t_pipeline *pipeline, t_next_logical_expr *next);
t_pipeline *create_pipeline(t_command *command, t_pipeline *next);
t_command *create_simple_command_node(t_simple_command *simple_command);
t_command *create_composite_command_node(t_logical_expr *logical_expr, t_redirection_list *redirection_list);
t_simple_command *create_simple_command(t_command_element *element, t_command_element *next);
t_command_element *create_identifier_element(char *identifier);
t_command_element *create_redirection_element(t_redirection *redirection);
t_redirection_list *create_redirection_list(t_redirection *redirection, t_redirection_list *next);
t_redirection *create_redirection(t_redirection_type type, char *file);

#endif