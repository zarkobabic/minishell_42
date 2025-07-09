/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 19:15:19 by zbabic            #+#    #+#             */
/*   Updated: 2025/07/09 20:08:47 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include <stdlib.h>

/// @brief Creates a new lexer for source lexing purpose
/// @param source Source buffer for reading, if NULL returns NULL
/// @return FAIL: NULL |
/// SUCCESS: reference to newly created lexer on heap
t_lexer *init_lexer(char *source)
{
	t_lexer *new_lexer;
	
	if(!source)
		return NULL;
	new_lexer = (t_lexer *)malloc(sizeof(t_lexer));
	if(!new_lexer)
		return NULL;
	new_lexer->source = source;
	new_lexer->in_quotes = false;
	new_lexer->length = ft_strlen(source);
	new_lexer->current_char = '\0';
	new_lexer->pos = 0;
	new_lexer->quote_state = STATE_NO_QUOTE;
	return new_lexer;
}

/// @brief Destroy the lexer (free its heap allocated space)
/// if the reference exists
/// @param lexer_ref Reference for lexer to destroy
void	destroy_lexer(t_lexer **lexer_ref)
{
	if(lexer_ref && (*lexer_ref))
	{
		free(*lexer_ref);
		*lexer_ref = NULL;
	}
}

void	reset_lexer(t_lexer *lexer, char *source)
{	
	lexer->source = source;
	lexer->in_quotes = false;
	lexer->length = ft_strlen(source);
	lexer->current_char = '\0';
	lexer->pos = 0;
	lexer->quote_state = STATE_NO_QUOTE;
	return lexer;
}

