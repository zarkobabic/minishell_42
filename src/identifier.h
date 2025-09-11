/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   identifier.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 00:19:46 by zbabic            #+#    #+#             */
/*   Updated: 2025/09/11 04:20:31 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IDENTIFIER_H
# define IDENTIFIER_H

# include "exe_environment.h"
# include <stdbool.h>

typedef struct s_argument_linked_list_node
{
	char								*string_value;
	struct s_argument_linked_list_node	*next;
}										t_arg_node;

typedef struct s_argument_list
{
	t_arg_node							*head;
	t_arg_node							*tail;
	long								size;
}										t_arg_list;

//arg_list

void									arg_list_initialize(t_arg_list *list);
t_arg_node								*arg_node_create(char *string_value);
bool									arg_list_append(t_arg_list *list,
											char *string_value);
void									arg_list_free_shallow(t_arg_list *list);
char									**arg_list_to_array_shallow(t_arg_list *list);
void									arg_list_free_deep(t_arg_list *list);

//identifier_expansion
char									*identifier_expand_variables_in_string(char *str,
											t_env *env);
char									**split_by_unquoted_space(const char *str);
void									free_split(char **split);
char									*remove_quotes(char *str);

bool									wildcard_check_if_matches(char *input,
											char *pattern);
char									**get_all_matched_names(char *pattern);
bool									is_containing_quotes(char *str);

#endif
