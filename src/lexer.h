/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 19:04:16 by zbabic            #+#    #+#             */
/*   Updated: 2025/07/11 17:23:07 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H

#include <stdbool.h>
#include "tokens.h"
#include "../libft/libft.h"

typedef struct s_token
{
	char *value;
	t_token_type type;
} t_token;

typedef enum e_quote_state
{
	STATE_NO_QUOTE,
	STATE_SINGLE_QUOTE,
	STATE_DOUBLE_QUOTE,
} t_quote_state;

typedef struct s_lexer{
    char *source;
    long pos;
    long length;
    char current_char;
    t_quote_state quote_state;
} t_lexer;

void	init_lexer(t_lexer *lexer, char *source);
void	destroy_lexer(t_lexer **lexer_ref);
t_lexer	*create_lexer();


#define LEXER_H
#endif