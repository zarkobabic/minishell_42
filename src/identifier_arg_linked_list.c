/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   identifier_arg_linked_list.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 15:14:33 by zbabic            #+#    #+#             */
/*   Updated: 2025/09/10 22:25:56 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "identifier.h"
#include <stdlib.h>

/// @brief Initialize an empty argument list
/// @param list Pointer to the list structure to initialize
void	arg_list_initialize(t_arg_list *list)
{
	if (list)
	{
		list->head = NULL;
		list->tail = NULL;
		list->size = 0;
	}
}

/// @brief Creates a new argument node with the given string value
/// @param string_value The string to store in the node,
/// (will not be duplicated, original will be used)
/// @return Pointer to the new node, or NULL if allocation fails
t_arg_node	*arg_node_create(char *string_value)
{
	t_arg_node	*new_node;

	new_node = malloc(sizeof(t_arg_node));
	if (!new_node)
		return (NULL);
	new_node->string_value = string_value;
	new_node->next = NULL;
	return (new_node);
}

/// @brief Appends a new node to the end of the linked list
/// @param list Pointer to the list structure containing head and tail
/// @param string_value The string to add to the list
/// @return true on success, false on failure
bool	arg_list_append(t_arg_list *list, char *string_value)
{
	t_arg_node	*new_node;

	if (!list || !string_value)
		return (false);
	new_node = arg_node_create(string_value);
	if (!new_node)
		return (free(string_value), false);
	if (!list->head)
		list->head = new_node;
	else
		list->tail->next = new_node;
	list->tail = new_node;
	list->size += 1;
	return (true);
}

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

char	**arg_list_to_array_shallow(t_arg_list *list)
{
	char		**array;
	t_arg_node	*current;
	long		i;

	if (!list || !list->head || list->size <= 0)
		return (NULL);
	array = malloc(sizeof(char *) * (list->size + 1));
	if (!array)
		return (NULL);
	current = list->head;
	i = 0;
	while (current && i < list->size)
	{
		array[i] = current->string_value;
		current = current->next;
		i++;
	}
	array[list->size] = NULL;
	return (array);
}
