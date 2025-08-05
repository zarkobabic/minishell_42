/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_tree_translator.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 18:19:01 by zbabic            #+#    #+#             */
/*   Updated: 2025/08/05 19:06:39 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_TREE_TRANSLATOR_H
# define AST_TREE_TRANSLATOR_H

# include "../libft/libft.h"
# include "ast_tree_structures.h"
# include <stdbool.h>
# include <stdlib.h>

// AST nodes constructor

t_ast_node			*ast_create_identifier(char *value);
t_ast_node			*ast_create_redirection(t_redirection_type type,
						char *target);
t_ast_node			*ast_create_cmd(void);
t_ast_node			*ast_create_subshell(void);
t_ast_node			*ast_create_pipeline(void);
t_ast_node			*ast_create_logical_expr(t_logical_expr_type type);

// AST nodes destructor

void				ast_free_identifier(t_ast_node **ast_identifier);
void				ast_free_redirection(t_ast_node **ast_redirection);
void				ast_free_redirection_list(
						t_ast_node *redirection_list_head);
void				ast_free_identifier_list(
						t_ast_node *identifier_list_head);
void				ast_free_command(t_ast_node **ast_command);
void				ast_free_subshell(t_ast_node **ast_subshell);
void				ast_free_pipeline(t_ast_node **ast_pipeline);
void				ast_free_logical_expr(
						t_ast_node **ast_logical_expr);
void				ast_free_all_logical_expr(
						t_ast_node *ast_logical_expr_head);
void				ast_free_tree(t_ast_node *root);

// Parse Tree -> AST tree tranlsator

// Forward declarations
void				ast_add_identifier_list_to_cmd(
						t_ast_node *ast_identifier_curr, t_ast_node *command);
t_ast_node			*ast_parse_identifier(char *string_value);
void				ast_add_redirection_to_cmd(t_ast_node *redirection,
						t_ast_node *command);
bool				ast_parse_command_element(t_command_element *element,
						t_ast_node *ast_cmd);
t_ast_node			*ast_parse_cmd(t_command *command);
t_ast_node			*ast_parse_redirection(t_redirection *redirection);
bool				ast_parse_redirection_list(t_ast_node *ast_subshell,
						t_redirection_list *redirection_list);
t_ast_node			*ast_parse_subshell(t_command *command);
bool				ast_parse_all_commands_for_pipeline(
						t_ast_node *ast_new_pipeline, t_pipeline *pipeline);
t_ast_node			*ast_parse_logical_expr(t_pipeline *pipeline,
						t_logical_expr_type type);
t_ast_node			*ast_parse_all_logical_expr(t_logical_expr *logical_expr);
t_ast_node			*ast_create_tree(t_command_line *parse_tree);

// Printing helpers - delete later

t_ast_node			*ast_create_tree(t_command_line *parse_tree);
void				ast_tree_print(t_ast_node *root);

#endif