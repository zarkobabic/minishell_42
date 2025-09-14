/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_tree_translator_expressions.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 14:34:42 by zbabic            #+#    #+#             */
/*   Updated: 2025/08/05 19:04:26 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast_tree_translator.h"

t_ast_node	*ast_parse_logical_expr(t_pipeline *pipeline,
		t_logical_expr_type type)
{
	t_ast_node	*ast_new_logical_expr;
	t_ast_node	*ast_new_pipeline;

	ast_new_logical_expr = ast_create_logical_expr(type);
	if (!ast_new_logical_expr)
		return (NULL);
	ast_new_pipeline = ast_create_pipeline();
	if (!ast_new_pipeline)
		return (ast_free_logical_expr(&ast_new_logical_expr), NULL);
	ast_new_logical_expr->data.logical_expr.pipeline = ast_new_pipeline;
	if (!ast_parse_all_commands_for_pipeline(ast_new_pipeline, pipeline))
		return (ast_free_pipeline(&ast_new_pipeline),
			ast_free_logical_expr(&ast_new_logical_expr), NULL);
	return (ast_new_logical_expr);
}

static t_ast_node	*ast_parse_first_logical_expr(t_logical_expr *logical_expr)
{
	t_logical_expr_type	type;

	type = TYPE_END;
	if (logical_expr->next_logical_expr)
		type = logical_expr->next_logical_expr->op_type;
	return (ast_parse_logical_expr(logical_expr->pipeline, type));
}

t_ast_node	*ast_parse_all_logical_expr(t_logical_expr *logical_expr)
{
	t_ast_node			*ast_logical_expr_head;
	t_ast_node			*ast_curr_logical_expr;
	t_next_logical_expr	*curr_next_logical_expr;
	t_logical_expr_type	type;

	if (!logical_expr)
		return (NULL);
	ast_logical_expr_head = ast_parse_first_logical_expr(logical_expr);
	if (!ast_logical_expr_head)
		return (NULL);
	curr_next_logical_expr = logical_expr->next_logical_expr;
	ast_curr_logical_expr = ast_logical_expr_head;
	while (curr_next_logical_expr)
	{
		type = TYPE_END;
		if (curr_next_logical_expr->next_logical_expr)
			type = curr_next_logical_expr->next_logical_expr->op_type;
		ast_curr_logical_expr->next = ast_parse_logical_expr(
				curr_next_logical_expr->pipeline, type);
		ast_curr_logical_expr = ast_curr_logical_expr->next;
		if (!ast_curr_logical_expr)
			return (ast_free_all_logical_expr(ast_logical_expr_head), NULL);
		curr_next_logical_expr = curr_next_logical_expr->next_logical_expr;
	}
	return (ast_logical_expr_head);
}

t_ast_node	*ast_create_tree(t_command_line *parse_tree)
{
	t_ast_node	*root;

	root = ast_parse_all_logical_expr(parse_tree->logical_expr);
	if (!root)
		return (NULL);
	return (root);
}
