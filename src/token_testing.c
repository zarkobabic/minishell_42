/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_testing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 17:36:12 by zbabic            #+#    #+#             */
/*   Updated: 2025/07/18 15:06:57 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "token.h"

// int main()
// {
    
//     t_token *token_simple = NULL;
//     t_token *token_composite = NULL;
    
//     token_simple = create_token(TOKEN_IDENTIFIER);
// 	if(!token_simple)
// 		return (1);
// 	if(token_add_value(token_simple, "someident", 9, COMPONENT_NOT_COMPOSITE) == TOKEN_ADD_ERROR_VALUE_MALLOC_FAILED)
// 		return (free_token(&token_simple), 1);
// 	token_composite = create_token(TOKEN_DOUBLE_QUOTE);
// 	if(!token_composite)
// 		return (free_token(&token_simple), 1);
// 	if(token_add_value(token_composite, "koko", 4, COMPONENT_LITERAL) == TOKEN_ADD_ERROR_VALUE_MALLOC_FAILED)
// 		return (free_token(&token_simple), free_token(&token_composite), 1);
// 	if(token_add_value(token_composite, "$LA", 3, COMPONENT_ENV_VAR) == TOKEN_ADD_ERROR_VALUE_MALLOC_FAILED)
// 		return (free_token(&token_simple), free_token(&token_composite), 1);
// 	if(token_add_value(token_composite, "$?", 4, COMPONENT_EXIT_STATUS) == TOKEN_ADD_ERROR_VALUE_MALLOC_FAILED)
// 		return (free_token(&token_simple), free_token(&token_composite), 1);
// 	token_print(token_simple);
// 	token_print(token_composite);
// 	free_token(&token_simple);
// 	free_token(&token_composite);
//     return (0);
// }

// #include "token.h"

// int main()
// {
    
//     t_token *token_simple = NULL;
// 	t_token *token_simple_without_value = NULL;
    
// 	token_simple = token_create_simple_with_value(TOKEN_RPAR, "someident", 9);
// 	if(!token_simple)
// 		return 1;
// 	token_simple_without_value = create_token(TOKEN_IDENTIFIER);
// 	if(!token_simple_without_value)
// 		return (free_token(&token_simple), 1);
//     token_add_value(token_simple_without_value, "kokola", 4, COMPONENT_LITERAL);
//     token_add_value(token_simple_without_value, "HOME", 4, COMPONENT_ENV_VAR);
//     token_add_value(token_simple_without_value, "nana*ja", 6, COMPONENT_LITERAL_WILDCARD);
// 	token_print(token_simple);
// 	token_print(token_simple_without_value);
// 	free_token(&token_simple);
// 	free_token(&token_simple_without_value);
//     return (0);
// }