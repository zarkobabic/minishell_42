/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   identifier_argument.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 03:24:57 by zbabic            #+#    #+#             */
/*   Updated: 2025/09/11 20:16:57 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "identifier.h"
#include <stdlib.h>

static char	**expand_and_split_argument(char *string_value, t_env *env)
{
	char	*expanded_string;
	char	**splitted_string;

	expanded_string = identifier_expand_variables_in_string(string_value, env, false);
	if (!expanded_string)
		return (NULL);
	splitted_string = split_by_unquoted_space(expanded_string);
	free(expanded_string);
	return (splitted_string);
}

static bool	process_splitted_string_element(char *splitted_string_el,
		t_arg_list *list)
{
	char	**temp_matched;
	long	i;
	char	*temp;

	temp_matched = get_all_matched_names(splitted_string_el);
	if (!temp_matched)
		return (false);
	i = 0;
	while (temp_matched[i])
	{
		temp = remove_quotes(temp_matched[i]);
		if (!temp)
			return (free_split(temp_matched), false);
		arg_list_append(list, temp);
		++i;
	}
	free_split(temp_matched);
	return (true);
}

static bool	handle_argument_processing(t_ast_node *args, t_arg_list *list,
		t_env *env)
{
	char	**splitted_string;
	long	i;

	while (args)
	{
		splitted_string = expand_and_split_argument(
				args->data.identifier.string_value, env);
		if (!splitted_string)
			return (false);
		i = 0;
		while (splitted_string[i])
		{
			if (!process_splitted_string_element(splitted_string[i], list))
				return (free_split(splitted_string), false);
			++i;
		}
		free_split(splitted_string);
		args = args->next;
	}
	return (true);
}

char	**identifier_process_arguments(t_ast_node *args, long *argc, t_env *env)
{
	t_arg_list	arg_list;
	char		**result_array;

	arg_list_initialize(&arg_list);
	if (!handle_argument_processing(args, &arg_list, env))
		return (arg_list_free_deep(&arg_list), NULL);
	result_array = arg_list_to_array_shallow(&arg_list);
	*argc = arg_list.size;
	arg_list_free_shallow(&arg_list);
	return (result_array);
}
