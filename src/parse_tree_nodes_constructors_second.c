/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tree_nodes_constructors_second.c             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 03:11:38 by zbabic            #+#    #+#             */
/*   Updated: 2025/07/26 04:09:30 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdlib.h>

t_simple_command	*create_simple_command(t_command_element *element,
		t_command_element *next)
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

t_command_element	*create_redirection_element(t_redirection *redirection)
{
	t_command_element	*element;

	element = malloc(sizeof(t_command_element));
	if (!element)
		return (NULL);
	element->type = TYPE_REDIRECTION;
	element->command_element.redirection = redirection;
	return (element);
}

t_redirection_list	*create_redirection_list(t_redirection *redirection,
		t_redirection_list *next)
{
	t_redirection_list	*list;

	list = malloc(sizeof(t_redirection_list));
	if (!list)
		return (NULL);
	list->redirection = redirection;
	list->next = next;
	return (list);
}

t_redirection	*create_redirection(t_redirection_type type, char *file)
{
	t_redirection	*redir;

	redir = malloc(sizeof(t_redirection));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->file = file;
	return (redir);
}
