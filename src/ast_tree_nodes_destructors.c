/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_tree_nodes_destructors.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 18:38:24 by zbabic            #+#    #+#             */
/*   Updated: 2025/08/05 18:40:39 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast_tree_translator.h"

void	ast_free_subshell(t_ast_node **ast_subshell)
{
	if (!ast_subshell || !(*ast_subshell))
		return ;
	ast_free_redirection_list((*ast_subshell)->data.subshell.redirections);
	ast_free_all_logical_expr((*ast_subshell)->data.subshell.logical_expr);
	free(*ast_subshell);
	*ast_subshell = NULL;
}

void	ast_free_pipeline(t_ast_node **ast_pipeline)
{
	t_ast_node	*ast_curr_cmd;
	t_ast_node	*ast_next_cmd;

	if (!ast_pipeline || !(*ast_pipeline))
		return ;
	ast_curr_cmd = (*ast_pipeline)->data.pipeline.cmds;
	while (ast_curr_cmd)
	{
		ast_next_cmd = ast_curr_cmd->next;
		if (ast_curr_cmd->type == AST_CMD)
			ast_free_command(&ast_curr_cmd);
		else
			ast_free_subshell(&ast_curr_cmd);
		ast_curr_cmd = ast_next_cmd;
	}
	free(*ast_pipeline);
	*ast_pipeline = NULL;
}

void	ast_free_logical_expr(t_ast_node **ast_logical_expr)
{
	t_ast_node	*ast_curr_pipeline;
	t_ast_node	*ast_next_pipeline;

	if (!ast_logical_expr || !(*ast_logical_expr))
		return ;
	ast_curr_pipeline = (*ast_logical_expr)->data.logical_expr.pipeline;
	while (ast_curr_pipeline)
	{
		ast_next_pipeline = ast_curr_pipeline->next;
		ast_free_pipeline(&ast_curr_pipeline);
		ast_curr_pipeline = ast_next_pipeline;
	}
	free(*ast_logical_expr);
	*ast_logical_expr = NULL;
}

void	ast_free_all_logical_expr(t_ast_node *ast_logical_expr_head)
{
	t_ast_node	*curr;

	curr = ast_logical_expr_head;
	while (curr)
	{
		ast_logical_expr_head = ast_logical_expr_head->next;
		ast_free_logical_expr(&curr);
		curr = ast_logical_expr_head;
	}
}

void	ast_free_tree(t_ast_node *root)
{
	if (!root)
		return ;
	ast_free_all_logical_expr(root);
}
