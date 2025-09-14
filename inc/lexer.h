/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 19:04:16 by zbabic            #+#    #+#             */
/*   Updated: 2025/07/31 03:54:46 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H

# include "../libft/libft.h"
# include "token.h"
# include <stdbool.h>
# include <stdio.h>

typedef struct s_lexer
{
	char	*source;
	long	pos;
	long	length;
	char	current_char;
	long	token_start_pos;
	t_token	*peeked_token;
	bool	has_peeked;
}			t_lexer;

// lexer_lifecycle

t_lexer		*lexer_create(void);
void		lexer_init(t_lexer *lexer, char *source);
void		lexer_destroy(t_lexer **lexer_ref);

// lexer_get_next_token

t_token		*lexer_get_next_token(t_lexer *lexer);

// lexer_operations

void		lexer_advance(t_lexer *lexer);
t_token		*lexer_advance_and_return_new_token(t_lexer *lexer,
				t_token_type type);
t_token		*lexer_peek_next_token(t_lexer *lexer);
void		lexer_consume_and_free_next_token(t_lexer *lexer);

// lexer utilities

bool		lexer_isspace(char c);
bool		lexer_is_special_symbol(char c);
void		lexer_skip_whitespaces(t_lexer *lexer);

# define LEXER_H
#endif