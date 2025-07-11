/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 18:43:42 by zbabic            #+#    #+#             */
/*   Updated: 2025/07/09 19:18:01 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENS_H
#define TOKENS_H

typedef enum e_token_type
{
	TOKEN_UNDEFINED, //ERROR
	TOKEN_AND, //&& BONUS
	TOKEN_OR, //|| BONUS (GREATER PRESEDENCE THAN |)
	TOKEN_WILDCARD, //* BONUS
	TOKEN_SINGLE_QUOTE, // '
	TOKEN_DOUBLE_QUOTE, // ""
	TOKEN_REDIRECT_IN, // <
	TOKEN_REDIRECT_OUT, // >
	TOKEN_HEREDOC, // <<
	TOKEN_REDIRECT_APPEND, // >>
	TOKEN_PIPE, // |
	TOKEN_VARIABLE, // $
	TOKEN_IDENT //INDENTIFIER
}	t_token_type;

#endif