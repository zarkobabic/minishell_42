/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_tree_nodes_constructors.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 14:27:37 by zbabic            #+#    #+#             */
/*   Updated: 2025/08/05 18:35:32 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast_tree_translator.h"

t_ast_node	*ast_create_pipeline(void)
{
	t_ast_node	*node;

	node = malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	ft_memset(node, 0, sizeof(t_ast_node));
	node->type = AST_PIPELINE;
	return (node);
}

t_ast_node	*ast_create_logical_expr(t_logical_expr_type type)
{
	t_ast_node	*node;

	node = malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	ft_memset(node, 0, sizeof(t_ast_node));
	node->type = AST_LOGICAL_EXPR;
	node->data.logical_expr.type = type;
	return (node);
}
