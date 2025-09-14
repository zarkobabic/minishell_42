/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_tree_translator_commands.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 18:51:47 by zbabic            #+#    #+#             */
/*   Updated: 2025/08/05 18:56:41 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast_tree_translator.h"

t_ast_node	*ast_parse_cmd(t_command *command)
{
	t_ast_node			*ast_cmd;
	t_simple_command	*curr_simple_command;

	if (!command)
		return (NULL);
	ast_cmd = ast_create_cmd();
	if (!ast_cmd)
		return (NULL);
	curr_simple_command = command->command.simple_command;
	while (curr_simple_command)
	{
		if (!ast_parse_command_element(curr_simple_command->command_element,
				ast_cmd))
			return (ast_free_command(&ast_cmd), NULL);
		curr_simple_command = curr_simple_command->simple_command;
	}
	return (ast_cmd);
}

bool	ast_parse_redirection_list(t_ast_node *ast_subshell,
		t_redirection_list *redirection_list)
{
	t_redirection_list	*curr_redirection_list;
	t_ast_node			*ast_parsed_redirection;
	t_ast_node			*ast_last_parsed_redirection;

	if (!redirection_list)
		return (true);
	ast_last_parsed_redirection = NULL;
	curr_redirection_list = redirection_list;
	while (curr_redirection_list)
	{
		ast_parsed_redirection = ast_parse_redirection(
				curr_redirection_list->redirection);
		curr_redirection_list->redirection->file = NULL;
		if (!ast_parsed_redirection)
			return (false);
		if (!ast_last_parsed_redirection)
			ast_subshell->data.subshell.redirections = ast_parsed_redirection;
		else
			ast_last_parsed_redirection->next = ast_parsed_redirection;
		ast_last_parsed_redirection = ast_parsed_redirection;
		curr_redirection_list = curr_redirection_list->next;
	}
	return (true);
}

t_ast_node	*ast_parse_subshell(t_command *command)
{
	t_ast_node	*ast_subshell;
	t_ast_node	*ast_logical_expr;

	ast_subshell = ast_create_subshell();
	if (!ast_subshell)
		return (NULL);
	if (!ast_parse_redirection_list(ast_subshell,
			command->command.composite_command->redirection_list))
		return (ast_free_subshell(&ast_subshell), NULL);
	ast_logical_expr = ast_parse_all_logical_expr(
			command->command.composite_command->logical_expr);
	if (!ast_logical_expr)
		return (ast_free_subshell(&ast_subshell), NULL);
	ast_subshell->data.subshell.logical_expr = ast_logical_expr;
	return (ast_subshell);
}

bool	ast_parse_all_commands_for_pipeline(t_ast_node *ast_new_pipeline,
		t_pipeline *pipeline)
{
	t_pipeline	*curr_pipeline;
	t_ast_node	*ast_curr_command;
	t_ast_node	*ast_last_command;

	ast_last_command = NULL;
	curr_pipeline = pipeline;
	while (curr_pipeline)
	{
		if (curr_pipeline->command->type == TYPE_COMPOSITE_COMMAND)
			ast_curr_command = ast_parse_subshell(curr_pipeline->command);
		else
			ast_curr_command = ast_parse_cmd(curr_pipeline->command);
		if (!ast_curr_command)
			return (false);
		if (!ast_last_command)
			ast_new_pipeline->data.pipeline.cmds = ast_curr_command;
		else
			ast_last_command->next = ast_curr_command;
		ast_new_pipeline->data.pipeline.cmd_cnt += 1;
		ast_last_command = ast_curr_command;
		curr_pipeline = curr_pipeline->pipeline;
	}
	return (true);
}
