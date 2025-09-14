/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_tree_translator_command_elements.c             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 18:48:56 by zbabic            #+#    #+#             */
/*   Updated: 2025/09/14 14:21:30 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast_tree_translator.h"

void	ast_add_identifier_list_to_cmd(t_ast_node *ast_identifier_curr,
		t_ast_node *command)
{
	if (ast_identifier_curr)
	{
		if (!command->data.command.args)
			command->data.command.args = ast_identifier_curr;
		else
			command->data.command.args_tail->next = ast_identifier_curr;
		command->data.command.args_tail = ast_identifier_curr;
		command->data.command.arg_cnt += 1;
		ast_identifier_curr = ast_identifier_curr->next;
	}
}

t_ast_node	*ast_parse_redirection(t_redirection *redirection)
{
	t_ast_node	*ast_redirection;

	if (!redirection)
		return (NULL);
	ast_redirection = ast_create_redirection(redirection->type,
			redirection->file);
	if (!ast_redirection)
		return (NULL);
	return (ast_redirection);
}

t_ast_node	*ast_parse_identifier(char *string_value)
{
	t_ast_node	*identifier_node;

	identifier_node = ast_create_identifier(string_value);
	return (identifier_node);
}

void	ast_add_redirection_to_cmd(t_ast_node *redirection, t_ast_node *command)
{
	if (!command->data.command.redirections)
		command->data.command.redirections = redirection;
	else
		command->data.command.redirections_tail->next = redirection;
	command->data.command.redirections_tail = redirection;
}

bool	ast_parse_command_element(t_command_element *element,
		t_ast_node *ast_cmd)
{
	t_ast_node	*ast_redirection;
	t_ast_node	*ast_identifier;

	if (element->type == TYPE_REDIRECTION)
	{
		ast_redirection = ast_parse_redirection(
				element->command_element.redirection);
		if (!ast_redirection)
			return (false);
		element->command_element.redirection->file = NULL;
		ast_add_redirection_to_cmd(ast_redirection, ast_cmd);
		return (true);
	}
	else if (element->type == TYPE_TOKEN_IDENTIFIER)
	{
		ast_identifier = ast_parse_identifier(
				element->command_element.string_value);
		if (!ast_identifier)
			return (false);
		element->command_element.string_value = NULL;
		ast_add_identifier_list_to_cmd(ast_identifier, ast_cmd);
		return (true);
	}
	else
		return (false);
}
