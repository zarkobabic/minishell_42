/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 18:17:05 by zbabic            #+#    #+#             */
/*   Updated: 2025/07/18 21:26:02 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include <stdio.h>

/// @brief Creates and returns a token with specified type, value is set to NULL
/// @param type type of the token
/// @return FAIL: NULL(Failed to allocate space for new token)
/// | SUCCESS: New token
t_token	*create_token(t_token_type type)
{
	t_token	*new_token;

	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	if(type == TOKEN_IDENTIFIER)
	{
		new_token->value.composite_value.components_head  = NULL;
		new_token->value.composite_value.components_tail  = NULL;
		new_token->value.composite_value.component_count = 0;
	}
	else
		new_token->value.simple_value = NULL;
	new_token->type = type;
	return (new_token);
}

/// @brief Adds value to already existing token. 
/// Allocates the memory on the heap with size len + 1
/// and copies len characters from source to token value.
/// if its composite it will add a new string to the linked list
/// of string components.
/// @param token Token to add new value to
/// @param src input string that is currently lexing
/// @param len number of characters to copy from source to value of the token
/// @param string_component_type if the value to add is not part 
/// of the composite sequence then COMPONENT_NOT_COMPOSITE 
/// (equals NULL, or 0) is sent.
/// If the value is part of the composite sequence then COMPONENT_LITERAL, 
/// COMPONENT_ENV_VAR or COMPONENT_EXIT_STATUS is sent 
/// to describe the expansion if necessary.
/// @return FAIL: TOKEN_ADD_ERROR_VALUE_MALLOC_FAILED if there is not enough memory
/// TOKEN_ADD_WARNING_VALUE_SRC_DONT_EXIST if the source is NULL or len is less than 0.
/// SUCCESS: TOKEN_ADD_NO_PROBLEM
t_token_add_error	token_add_value(t_token *token, const char *src, long len, t_component_type string_component_type)
{
	char *new_value;

	if(!src || len < 0)
		return TOKEN_ADD_WARNING_VALUE_SRC_DONT_EXIST;
	new_value = (char *)malloc(len + 1);
	if (!new_value)
		return TOKEN_ADD_ERROR_VALUE_MALLOC_FAILED;
	ft_memcpy(new_value, src, len);
	new_value[len] = '\0';
	if(token->type == TOKEN_IDENTIFIER)
	{
		if(!token_composite_add_component(token, new_value, string_component_type))
			return (free(new_value), TOKEN_ADD_ERROR_STRING_COMPONENT_ALLOCATION_FAILED);
	}
	else
		token->value.simple_value = new_value;
	return TOKEN_ADD_NO_PROBLEM;
}

/// @brief Frees the given token
/// @param token
void	free_token(t_token **token)
{
	if (!token || !(*token))
		return ;
	else if ((*token)->type == TOKEN_IDENTIFIER)
		token_composite_delete_list(*token);
	else if((*token)->value.simple_value)
		free((*token)->value.simple_value);
	free(*token);
	*token = NULL;
}

/// @brief Creates simple token then try to add simple value to it 
/// by allocating len element from the source and terminating it 
/// and set the token value to be that newly created string 
/// @param type type of the token to be created
/// @param src source for the value
/// @param len length of the value
/// @return SUCCESS: Simple token that has type passed as argument, 
/// FAIL: NULL (Allocation for token or string failed)
t_token *token_create_simple_with_value(t_token_type type, const char *src, size_t len)
{
	t_token *token;

	token = create_token(type);
	if(!token)
		return (NULL);
	if(token_add_value(token, src, len, COMPONENT_NOT_COMPOSITE) == TOKEN_ADD_ERROR_VALUE_MALLOC_FAILED)
		return (free_token(&token), NULL);
	return token;
}

static void	token_print_type_to_string(t_token_type type, char *value)
{
	char *type_to_string;

	if(type == TOKEN_UNDEFINED)
		type_to_string = "TOKEN_UNDEFINED";
	else if(type == TOKEN_AND)
		type_to_string = "TOKEN_AND (&&)";
	else if(type == TOKEN_OR)
		type_to_string = "TOKEN_OR (||)";
	else if(type == TOKEN_REDIRECT_IN)
		type_to_string = "TOKEN_REDIRECT_IN (<)";
	else if(type == TOKEN_REDIRECT_OUT)
		type_to_string = "TOKEN_REDIRECT_OUT(>)";
	else if(type == TOKEN_REDIRECTION_HEREDOC)
		type_to_string = "TOKEN_REDIRECTION_HEREDOC(<<)";
	else if(type == TOKEN_REDIRECT_APPEND)
		type_to_string = "TOKEN_REDIRECT_APPEND(>>)";
	else if(type == TOKEN_PIPE)
		type_to_string = "TOKEN_PIPE(|)";
	else if(type == TOKEN_LPAR)
		type_to_string = "TOKEN_LPAR( ( )";
	else if(type == TOKEN_RPAR)
		type_to_string = "TOKEN_RPAR ( ) )";
	else
		type_to_string = "TOKEN_EOF";
	printf("TOKEN_SIMPLE: type: %s, value: %s\n", type_to_string, value);
}

void	token_print(t_token *token)
{
	t_string_component *temp;

	if(token->type == TOKEN_IDENTIFIER)
	{
		temp = token->value.composite_value.components_head;
		printf("TOKEN_COMPOSITE: type: TOKEN_IDENTIFIER, num_of_string_components: %ld\n", token->value.composite_value.component_count);
		while(temp)
		{
			printf("--->STRING_COMPONENT: type_string: %d, value: %s\n",temp->type, temp->value);
			temp = temp->next;
		}
	}
	else
		token_print_type_to_string(token->type, token->value.simple_value);
}
