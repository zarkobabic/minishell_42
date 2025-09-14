/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   identifier_arg_linked_list_cleanup.c               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 12:44:16 by zbabic            #+#    #+#             */
/*   Updated: 2025/09/14 13:13:52 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "identifier.h"
#include <stdlib.h>

/// @brief Free all nodes in the argument list (but doesnt free their strings)
/// @param list Pointer to the list structure to free
void	arg_list_free_shallow(t_arg_list *list)
{
	t_arg_node	*current;
	t_arg_node	*next;

	if (!list)
		return ;
	current = list->head;
	while (current)
	{
		next = current->next;
		free(current);
		current = next;
	}
	list->head = NULL;
	list->tail = NULL;
}

/// @brief Free all nodes in the argument list
/// @param list Pointer to the list structure to free
void	arg_list_free_deep(t_arg_list *list)
{
	t_arg_node	*current;
	t_arg_node	*next;

	if (!list)
		return ;
	current = list->head;
	while (current)
	{
		next = current->next;
		if (current->string_value)
			free(current->string_value);
		free(current);
		current = next;
	}
	list->head = NULL;
	list->tail = NULL;
}
