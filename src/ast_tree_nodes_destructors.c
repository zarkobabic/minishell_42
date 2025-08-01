/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_tree_nodes_destructors.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 15:46:41 by zbabic            #+#    #+#             */
/*   Updated: 2025/08/01 18:44:03 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast_tree_structures.h"

void	ast_free_identifier(t_ast_node **ast_identifier)
{
	if(!ast_identifier || !(*ast_identifier))
		return ;
	if((*ast_identifier)->data.identifier.string_value)
		free((*ast_identifier)->data.identifier.string_value);
	free(*ast_identifier);
	*ast_identifier = NULL;
}

void	ast_free_redirection(t_ast_node **ast_redirection)
{
	if(!ast_redirection || !(*ast_redirection))
		return ;
	if((*ast_redirection)->data.redirection.target)
		free((*ast_redirection)->data.redirection.target);
	free(*ast_redirection);
	*ast_redirection = NULL;
}

void	ast_free_redirection_list(t_ast_node *redirection_list_head)
{
	t_ast_node	*ast_curr_redirection;
	t_ast_node	*ast_next_redirection;

	ast_curr_redirection = redirection_list_head;
	while (ast_curr_redirection)
	{
		ast_next_redirection = ast_next_redirection->next;
		ast_free_redirection(&ast_curr_redirection);
		ast_curr_redirection = ast_next_redirection;
	}
}

void	ast_free_identifier_list(t_ast_node *identifier_list_head)
{
	t_ast_node	*ast_curr_identifier;
	t_ast_node	*ast_next_identifier;

	ast_curr_identifier = identifier_list_head;
	while (ast_curr_identifier)
	{
		ast_next_identifier = ast_next_identifier->next;
		ast_free_identifier(&ast_curr_identifier);
		ast_curr_identifier = ast_next_identifier;
	}
}

void	ast_free_command(t_ast_node **ast_command)
{
	if(!ast_command || !(*ast_command))
		return ;
	ast_free_redirection_list((*ast_command)->data.command.redirections);
	ast_free_identifier(&((*ast_command)->data.command.executable));
	ast_free_identifier_list((*ast_command)->data.command.args);
	free(*ast_command);
	*ast_command = NULL;
}

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
