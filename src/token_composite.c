/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   composite_tokens.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 17:09:48 by zbabic            #+#    #+#             */
/*   Updated: 2025/07/13 00:48:23 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

/// @brief Create new string component node for the list of string components
/// @param value value to add to new node
/// @param component_type type to add to new node
/// @return FAIL: NULL if allocation failed, SUCCESS: the component itself
t_string_component *token_composite_new_string_component(char *value, t_component_type component_type)
{
	t_string_component *new_string_component;

	new_string_component = (t_string_component *)malloc(sizeof(t_string_component));
	if(!new_string_component)
		return NULL;
	new_string_component->value = value;
	new_string_component->type = component_type;
	new_string_component->next = NULL;
	return new_string_component;
}

/// @brief Adds new component to the list of string components for the composite token.
/// @param token Token to add to
/// @param value Value to add to the string component
/// @param component_type Type of string component COMPONENT_LITERAL, COMPONENT_ENV_VAR or COMPONENT_EXIT_STATUS
/// @return FAIL: false if allocation failed, SUCCESS: true
bool	token_composite_add_component(t_token *token, char *value, t_component_type component_type)
{
	t_string_component *new_string_component;

	new_string_component = token_composite_new_string_component(value, component_type);
	if(!new_string_component)
		return false;
	if(token->value.composite_value.components_head)
	{
		token->value.composite_value.components_tail->next = new_string_component;
		token->value.composite_value.components_tail= new_string_component;
	}
	else
	{
		token->value.composite_value.components_head = new_string_component;
		token->value.composite_value.components_tail = new_string_component;
	}
	token->value.composite_value.component_count += 1;
	return true;
}

/// @brief Deletes the list of string_component for the given token
/// @param token Token to delete string component list from
void	token_composite_delete_list(t_token *token)
{
	t_string_component *temp;

	temp = token->value.composite_value.components_head;
	while(temp)
	{
		token->value.composite_value.components_head = token->value.composite_value.components_head->next;
		if(temp->value)
			free(temp->value);
		free(temp);
		temp = token->value.composite_value.components_head;
	}
}
