/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_tree_structures.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 18:58:15 by zbabic            #+#    #+#             */
/*   Updated: 2025/08/05 18:42:35 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_TREE_STRUCTURES_H
# define AST_TREE_STRUCTURES_H

# include "parse_tree_structures.h"

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
		// IDENTIFIER
		struct					s_identifier
		{
			char				*string_value;
		} identifier;
		// REDIRECTION node
		struct					s_ast_redirection
		{
			t_redirection_type	type;
			char				*target;
		} redirection;
		// AST_CMD node
		struct					s_ast_command
		{
			struct s_ast_node	*executable;
			struct s_ast_node	*args;
			struct s_ast_node	*args_tail;
			long				arg_cnt;
			struct s_ast_node	*redirections;
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
			struct s_ast_node	*cmds;
			long				cmd_cnt;
		} pipeline;
		// AST_LOGICAL_EXPR
		struct					s_ast_logical_expr
		{
			t_logical_expr_type	type;
			struct s_ast_node	*pipeline;
		} logical_expr;
	} data;
	struct s_ast_node			*next;
}								t_ast_node;

#endif
