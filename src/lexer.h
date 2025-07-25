/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 19:04:16 by zbabic            #+#    #+#             */
/*   Updated: 2025/07/25 14:24:28 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H

#include <stdbool.h>
#include <stdio.h>
#include "token.h"
#include "../libft/libft.h"

typedef struct s_lexer{
    char *source;
    long pos;
    long length;
    char current_char;
	long token_start_pos;
} t_lexer;

//lexer_lifecycle

t_lexer	*lexer_create();
void	lexer_init(t_lexer *lexer, char *source);
void	lexer_destroy(t_lexer **lexer_ref);

//lexer_get_next_token

t_token *lexer_get_next_token(t_lexer *lexer);

//lexer_utilities

bool	lexer_isspace(char c);
bool	lexer_is_special_symbol(char c);
void	lexer_advance(t_lexer *lexer);
t_token	*lexer_advance_and_return_new_token(t_lexer *lexer, t_token_type type);

#define LEXER_H
#endif