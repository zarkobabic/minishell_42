/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 19:04:16 by zbabic            #+#    #+#             */
/*   Updated: 2025/07/13 01:23:57 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H

#include <stdbool.h>
#include "token.h"
#include "../libft/libft.h"

typedef struct s_lexer{
    char *source;
    long pos;
    long length;
    char current_char;
	long token_start_pos; //starting position of token for strings
} t_lexer;

//lexer_lifecycle

t_lexer	*create_lexer();
void	init_lexer(t_lexer *lexer, char *source);
void	destroy_lexer(t_lexer **lexer_ref);

//lexer_operations

void	lexer_advance(t_lexer *lexer);
void	lexer_skip_whitespaces(t_lexer *lexer);
t_token *get_next_token(t_lexer *lexer);


#define LEXER_H
#endif