/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_tree_structures.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 18:58:15 by zbabic            #+#    #+#             */
/*   Updated: 2025/08/01 22:34:56 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_TREE_STRUCTURES_H
# define AST_TREE_STRUCTURES_H

# include "parse_tree_structures.h"
# include "../libft/libft.h"
# include <stdlib.h>

typedef enum e_ast_node_type
{
	AST_REDIRECTION,
	AST_CMD,
	AST_SUBSHELL,
	AST_PIPELINE,
	AST_LOGICAL_EXPR,
	AST_IDENTIFIER
}								t_ast_node_type;

// AST Node Structure
typedef struct s_ast_node
{
	t_ast_node_type				type;
	union						u_data
	{

		//IDENTIFIER
		struct					s_identifier
		{
			char				*string_value;
		} identifier;

		// REDIRECTION node
		struct					s_ast_redirection
		{
			t_redirection_type	type;
			char *target; // File or delimiter to redirect to
		} redirection;

		// AST_CMD node
		struct					s_ast_command
		{
			struct s_ast_node	*executable;
			struct s_ast_node	*args;
			struct s_ast_node	*args_tail;
			long				arg_cnt;
			struct s_ast_node 	*redirections; // redirections linked list
			struct s_ast_node	*redirections_tail;
		} command;

		// AST_SUBSHELL
		struct					s_ast_subshell
		{
			struct s_ast_node	*logical_expr;
			struct s_ast_node	*redirections;
		} subshell;

		// AST_PIPELINE node
		struct					s_ast_pipeline
		{
			struct s_ast_node 	*cmds; // Linked list of commands and subshells to execute
			long				cmd_cnt;
		} pipeline;

		// AST_LOGICAL_EXPR
		struct					s_ast_logical_expr
		{
			t_logical_expr_type	type;
			struct s_ast_node *pipeline; // first operand of logical_expression
		} logical_expr;

	} data;
	struct s_ast_node *next; // For linked lists
}								t_ast_node;

//AST nodes constructor

t_ast_node	*ast_create_identifier(char *value);
t_ast_node	*ast_create_redirection(t_redirection_type type, char *target);
t_ast_node	*ast_create_cmd(void);
t_ast_node	*ast_create_subshell(void);
t_ast_node	*ast_create_pipeline(void);
t_ast_node	*ast_create_logical_expr(t_logical_expr_type type);

//AST nodes destructor

void	ast_free_identifier(t_ast_node **ast_identifier);
void	ast_free_redirection(t_ast_node **ast_redirection);
void	ast_free_redirection_list(t_ast_node *redirection_list_head);
void	ast_free_identifier_list(t_ast_node *identifier_list_head);
void	ast_free_command(t_ast_node **ast_command);
void	ast_free_subshell(t_ast_node **ast_subshell);
void	ast_free_pipeline(t_ast_node **ast_pipeline);
void	ast_free_logical_expr(t_ast_node **ast_logical_expr);
void	ast_free_all_logical_expr(t_ast_node *ast_logical_expr_head);
void	ast_free_tree(t_ast_node *root);

//Printing helpers - delete later

t_ast_node	*ast_create_tree(t_command_line *parse_tree);
void	ast_tree_print(t_ast_node *root);

#endif
