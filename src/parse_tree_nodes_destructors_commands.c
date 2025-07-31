/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tree_nodes_destructors_commands.c            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 13:01:32 by zbabic            #+#    #+#             */
/*   Updated: 2025/07/31 00:10:51 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdlib.h>

void	free_command_element(t_command_element *command_element)
{
	if (!command_element)
		return ;
	if (command_element->type == TYPE_TOKEN_IDENTIFIER)
	{
		if (command_element->command_element.string_value)
			free(command_element->command_element.string_value);
	}
	else
	{
		if (command_element->command_element.redirection)
			free_redirection(command_element->command_element.redirection);
	}
	free(command_element);
}

void	free_composite_command(t_composite_command *composite_command)
{
	if (!composite_command)
		return ;
	if (composite_command->logical_expr)
		free_logical_expr(composite_command->logical_expr);
	if (composite_command->redirection_list)
		free_redirection_list(composite_command->redirection_list);
	free(composite_command);
}

void	free_simple_command(t_simple_command *simple_command)
{
	if (!simple_command)
		return ;
	if (simple_command->command_element)
		free_command_element(simple_command->command_element);
	if (simple_command->simple_command)
		free_simple_command(simple_command->simple_command);
	free(simple_command);
}

void	free_command(t_command *command)
{
	if (!command)
		return ;
	if (command->type == TYPE_SIMPLE_COMMAND)
	{
		if (command->command.simple_command)
			free_simple_command(command->command.simple_command);
	}
	else
	{
		if (command->command.composite_command)
			free_composite_command(command->command.composite_command);
	}
	free(command);
}
