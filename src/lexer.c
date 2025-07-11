/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 19:15:19 by zbabic            #+#    #+#             */
/*   Updated: 2025/07/11 17:21:54 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include <stdlib.h>

/// @brief Creates a new lexer
/// @param  
/// @return SUCCESS: reference to newly created lexer on heap
/// FAIL: NULL (allocation failed)
t_lexer *create_lexer(void)
{
	t_lexer *new_lexer;
	
	new_lexer = (t_lexer *)malloc(sizeof(t_lexer));
	if(!new_lexer)
		return NULL;
	return new_lexer;
}

/// @brief Sets passed lexer at the beginning of the passed argument source
/// @param lexer Pointer to lexer which should be used for lexing,
/// if NULL the function will just return without doing anything
/// @param source Source buffer for reading, if NULL it will set
/// lexer character to '\0'
void	init_lexer(t_lexer *lexer, char *source)
{
	if(!lexer)
		return ;
	lexer->source = source;
	lexer->length = ft_strlen(source);
	if(lexer->length > 0)
		lexer->current_char = source[0];
	else
		lexer->current_char = '\0';
	lexer->pos = 0;
	lexer->quote_state = STATE_NO_QUOTE;
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

long ft_min(long a, long b)
{
	if(b < a)
		return b;
	return a;
}

void	lexer_advance(t_lexer *lexer)
{
	lexer->pos = min(lexer->pos + 1, lexer->length);
	lexer->current_char = lexer->source[lexer->pos];
}

// void	skip_whitespaces(t_lexer *lexer, )
// {
// 	while (str[*counter] == '\t' || str[*counter] == '\n'
// 		|| str[*counter] == '\v' || str[*counter] == '\f'
// 		|| str[*counter] == '\r' || str[*counter] == ' ')
// 	*counter = *counter + 1;
// }
