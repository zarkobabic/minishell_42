/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_tree_nodes_destructors_commands.c              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 15:46:41 by zbabic            #+#    #+#             */
/*   Updated: 2025/08/05 18:39:26 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast_tree_translator.h"

void	ast_free_identifier(t_ast_node **ast_identifier)
{
	if (!ast_identifier || !(*ast_identifier))
		return ;
	if ((*ast_identifier)->data.identifier.string_value)
		free((*ast_identifier)->data.identifier.string_value);
	free(*ast_identifier);
	*ast_identifier = NULL;
}

void	ast_free_redirection(t_ast_node **ast_redirection)
{
	if (!ast_redirection || !(*ast_redirection))
		return ;
	if ((*ast_redirection)->data.redirection.target)
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
		ast_next_redirection = ast_curr_redirection->next;
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
		ast_next_identifier = ast_curr_identifier->next;
		ast_free_identifier(&ast_curr_identifier);
		ast_curr_identifier = ast_next_identifier;
	}
}

void	ast_free_command(t_ast_node **ast_command)
{
	if (!ast_command || !(*ast_command))
		return ;
	ast_free_redirection_list((*ast_command)->data.command.redirections);
	ast_free_identifier(&((*ast_command)->data.command.executable));
	ast_free_identifier_list((*ast_command)->data.command.args);
	free(*ast_command);
	*ast_command = NULL;
}
