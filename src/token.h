/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 18:43:42 by zbabic            #+#    #+#             */
/*   Updated: 2025/07/18 13:46:17 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENS_H
#define TOKENS_H

#include <stdbool.h>
#include "../libft/libft.h"

typedef enum e_token_add_problem
{
	TOKEN_ADD_NO_PROBLEM,
	TOKEN_ADD_ERROR_VALUE_MALLOC_FAILED,
	TOKEN_ADD_WARNING_VALUE_SRC_DONT_EXIST,
	TOKEN_ADD_ERROR_STRING_COMPONENT_ALLOCATION_FAILED
} t_token_add_error;

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

typedef enum e_component_type
{
	COMPONENT_NOT_COMPOSITE,
	COMPONENT_LITERAL_WILDCARD,
	COMPONENT_LITERAL,
	COMPONENT_ENV_VAR,
	COMPONENT_EXIT_STATUS //$? case
} t_component_type;

/// @brief One string component node in a lined list,
/// Value is non null for LITERAL and ENV_VAR, NULL for EXIT_STATUS($?)
typedef struct s_string_component{
    t_component_type type;
    char *value;
	struct s_string_component *next;
} t_string_component;

typedef struct s_composite
{
	t_string_component *components_head;
	t_string_component *components_tail;
	size_t component_count;
} t_composite;

/// @brief Union representing the value of the token.
/// It can be with simple_value for normal tokens, or
/// with struct with components linked list, if it has
/// multiple string components (VAR EXPANSION SCENARIO)
// typedef union u_value
// {
// 	char *simple_value;
// 	t_composite composite_value;
// } t_value;

typedef struct s_token
{
	t_token_type type;
	union u_value
	{
		char *simple_value;
		t_composite composite_value;
	} value;
} t_token;

// tokens

t_token	*create_token(t_token_type type);
t_token_add_error	token_add_value(t_token *token, const char *src, size_t len, t_component_type string_component_type);
void	free_token(t_token **token);
void	print_token(t_token *token);
t_token *token_create_simple_with_value(t_token_type type, const char *src, size_t len);

// composite_tokens

t_string_component *token_composite_new_string_component(char *value, t_component_type component_type);
bool	token_composite_add_component(t_token *token, char *value, t_component_type component_type);
void	token_composite_delete_list(t_token *token);

#endif