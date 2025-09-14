/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 18:17:05 by zbabic            #+#    #+#             */
/*   Updated: 2025/09/14 10:13:31 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

/// @brief Creates and returns a token with specified type, value is set to NULL
/// @param type type of the token
/// @return GENERAL_ERR: NULL(Failed to allocate space for new token)
/// | SUCCESS: New token
t_token	*token_create(t_token_type type)
{
	t_token	*new_token;

	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->value = NULL;
	new_token->type = type;
	return (new_token);
}

/// @brief Adds value to already existing token.
/// Allocates the memory on the heap with size len + 1
/// and copies len characters from source to token value.
/// @param token Token to add new value to
/// @param src input string that is currently lexing
/// @param len number of characters to copy from source to value of the token
/// @return GENERAL_ERR: false if the source is NULL or len is less than 0
///, or the allocation of the copy of the string failed.
/// SUCCESS: true
bool	token_add_value(t_token *token, const char *src, long len)
{
	char	*new_value;

	if (!src || len < 0)
		return (false);
	new_value = (char *)malloc(len + 1);
	if (!new_value)
		return (false);
	ft_memcpy(new_value, src, len);
	new_value[len] = '\0';
	token->value = new_value;
	return (true);
}

/// @brief Frees the given token
/// @param token Token to free
void	token_free(t_token **token)
{
	if (!token || !(*token))
		return ;
	else if ((*token)->value)
		free((*token)->value);
	free(*token);
	*token = NULL;
}

/// @brief Frees only the structure allocated 
/// for the token. Doesn't free the token value!
/// @param token Token to free
void	token_free_shallow(t_token **token)
{
	if (!token || !(*token))
		return ;
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
/// GENERAL_ERR: NULL (Allocation for token or string failed)
t_token	*token_create_simple_with_value(t_token_type type, const char *src,
		size_t len)
{
	t_token	*token;

	token = token_create(type);
	if (!token)
		return (NULL);
	if (!token_add_value(token, src, len))
		return (token_free(&token), NULL);
	return (token);
}
