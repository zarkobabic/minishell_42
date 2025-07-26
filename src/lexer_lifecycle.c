/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_lifecycle.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 19:15:19 by zbabic            #+#    #+#             */
/*   Updated: 2025/07/26 01:44:18 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include <stdlib.h>

/// @brief Creates a new lexer
/// @param
/// @return SUCCESS: reference to newly created lexer on heap
/// FAIL: NULL (allocation failed)
t_lexer	*lexer_create(void)
{
	t_lexer	*new_lexer;

	new_lexer = (t_lexer *)malloc(sizeof(t_lexer));
	if (!new_lexer)
		return (NULL);
	return (new_lexer);
}

/// @brief Sets passed lexer at the beginning of the passed argument source
/// @param lexer Pointer to lexer which should be used for lexing,
/// if NULL the function will just return without doing anything
/// @param source Source buffer for reading, if NULL it will set
/// lexer character to '\0' and lexer source to NULL
void	lexer_init(t_lexer *lexer, char *source)
{
	if (!lexer)
		return ;
	lexer->source = source;
	lexer->length = ft_strlen(source);
	if (lexer->length > 0)
		lexer->current_char = source[0];
	else
		lexer->current_char = '\0';
	lexer->has_peeked = false;
	lexer->peeked_token = NULL;
	lexer->pos = 0;
	lexer->token_start_pos = 0;
}

/// @brief Destroy the lexer (free its heap allocated space)
/// if the reference exists
/// @param lexer_ref Reference for lexer to destroy
void	lexer_destroy(t_lexer **lexer_ref)
{
	if (lexer_ref && (*lexer_ref))
	{
		if((*lexer_ref)->peeked_token)
			token_free(&((*lexer_ref)->peeked_token));
		free(*lexer_ref);
		*lexer_ref = NULL;
	}
}
