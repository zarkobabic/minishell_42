/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_testing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 17:36:12 by zbabic            #+#    #+#             */
/*   Updated: 2025/07/13 00:47:49 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "token.h"

// int main()
// {
    
//     t_token *token_simple = NULL;
//     t_token *token_composite = NULL;
    
//     token_simple = create_token(TOKEN_IDENTIFIER);
// 	token_add_value(token_simple, "someident", 9, COMPONENT_NOT_COMPOSITE);
// 	token_composite = create_token(TOKEN_DOUBLE_QUOTE);
// 	token_add_value(token_composite, "koko", 4, COMPONENT_LITERAL);
// 	token_add_value(token_composite, "$LA", 3, COMPONENT_ENV_VAR);
// 	token_add_value(token_composite, "$?", 4, COMPONENT_EXIT_STATUS);
// 	print_token(token_simple);
// 	print_token(token_composite);
// 	free_token(&token_simple);
// 	free_token(&token_composite);
//     return (0);
// }