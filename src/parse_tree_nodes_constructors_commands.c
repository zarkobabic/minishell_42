/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tree_nodes_constructors_commands.c           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 03:36:25 by zbabic            #+#    #+#             */
/*   Updated: 2025/07/31 04:42:51 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdlib.h>

t_command	*create_simple_command_node(t_simple_command *simple_command)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->type = TYPE_SIMPLE_COMMAND;
	cmd->command.simple_command = simple_command;
	return (cmd);
}

t_command	*create_composite_cmd(t_logical_expr *logical_expr,
		t_redirection_list *redirection_list)
{
	t_command			*cmd;
	t_composite_command	*composite;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	composite = malloc(sizeof(t_composite_command));
	if (!composite)
	{
		free(cmd);
		return (NULL);
	}
	composite->logical_expr = logical_expr;
	composite->redirection_list = redirection_list;
	cmd->type = TYPE_COMPOSITE_COMMAND;
	cmd->command.composite_command = composite;
	return (cmd);
}

t_simple_command	*create_simple_command(t_command_element *element,
		t_simple_command *next)
{
	t_simple_command	*cmd;

	cmd = malloc(sizeof(t_simple_command));
	if (!cmd)
		return (NULL);
	cmd->command_element = element;
	cmd->simple_command = next;
	return (cmd);
}

t_command_element	*create_identifier_element(char *identifier)
{
	t_command_element	*element;

	element = malloc(sizeof(t_command_element));
	if (!element)
		return (NULL);
	element->type = TYPE_TOKEN_IDENTIFIER;
	element->command_element.string_value = identifier;
	return (element);
}
