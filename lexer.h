/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 19:04:16 by zbabic            #+#    #+#             */
/*   Updated: 2025/07/09 19:35:21 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H

#include <stdbool.h>
#include "tokens.h"
#include "libft.h"

typedef struct s_token
{
	char *value;
	t_token_type type;
} t_token;

typedef enum e_quote_state
{
	STATE_SINGLE_QUOTE = 1,
	STATE_DOUBLE_QUOTE,
	STATE_NO_QUOTE
} t_quote_state;

typedef struct s_lexer{
    char *source;
    long pos;
    long length;
    char current_char;
    bool in_quotes;
    t_quote_state quote_state;
} t_lexer;

t_lexer *init_lexer(char *source);
void	destroy_lexer(t_lexer **lexer_ref);

#define LEXER_H
#endif