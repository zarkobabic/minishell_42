/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tree_nodes_constructors_redirections.        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 03:11:38 by zbabic            #+#    #+#             */
/*   Updated: 2025/07/31 04:41:54 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdlib.h>

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
