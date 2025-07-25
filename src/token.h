/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 18:43:42 by zbabic            #+#    #+#             */
/*   Updated: 2025/07/24 21:44:22 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENS_H
#define TOKENS_H

#include <stdbool.h>
#include "../libft/libft.h"

typedef enum e_token_type
{
	TOKEN_UNDEFINED, //ERROR
	TOKEN_AND, //&& BONUS
	TOKEN_OR, //|| BONUS (GREATER PRESEDENCE THAN |)
	TOKEN_REDIRECT_IN, // <
	TOKEN_REDIRECT_OUT, // >
	TOKEN_REDIRECTION_HEREDOC, // <<
	TOKEN_REDIRECT_APPEND, // >>
	TOKEN_PIPE, // |
	TOKEN_LPAR, // (
	TOKEN_RPAR, // )
	TOKEN_IDENTIFIER, //INDENTIFIER
	TOKEN_EOF
}	t_token_type;

typedef struct s_token
{
	t_token_type type;
	char *value;
} t_token;

// tokens

t_token	*token_create(t_token_type type);
bool	token_add_value(t_token *token, const char *src, long len);
void	token_free(t_token **token);
void	token_print(t_token *token);
t_token *token_create_simple_with_value(t_token_type type, const char *src, size_t len);

#endif