/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   identifier.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 00:19:46 by zbabic            #+#    #+#             */
/*   Updated: 2025/09/14 14:02:02 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IDENTIFIER_H
# define IDENTIFIER_H

# include "exe_environment.h"
# include <stdbool.h>

# define SINGLE_QUOTES 0
# define DOUBLE_QUOTES 1
# define NEW 0
# define ORIGINAL 1

typedef struct s_argument_linked_list_node
{
	char								*string_value;
	struct s_argument_linked_list_node	*next;
}	t_arg_node;

typedef struct s_argument_list
{
	t_arg_node	*head;
	t_arg_node	*tail;
	long		size;
}	t_arg_list;

// arg_list

void	arg_list_initialize(t_arg_list *list);
// t_arg_node	*arg_node_create(char *string_value); TODO: delete
bool	arg_list_append(t_arg_list *list, char *string_value);
void	arg_list_free_shallow(t_arg_list *list);
char	**arg_list_to_array_shallow(t_arg_list *list);
void	arg_list_free_deep(t_arg_list *list);

// identifier_expansion

char	*identifier_expand_variables_in_string(char *str, t_env *env,
			bool heredoc_single_quotes);
char	*identifier_expand_environment_variables(char *str, long new_size,
			t_env *env, bool heredoc_single_quotes);
bool	is_containing_quotes(char *str);
int		count_digits(int num);
long	get_env_var_name_len(char *str);
long	identifier_count_new_string_len(char *str, t_env *env,
			bool heredoc_single_quotes);
void	copy_regular_char(char c, char *new_str, long *new_cnt,
			bool *in_quotes);
char	*get_env_var_value(char *str, long var_name_len, bool *error_flag,
			t_env *env);
long	count_env_var(char *str, long var_name_len, t_env *env);
char	**split_by_unquoted_space(const char *str);
long	get_word_len(const char *str, long *i);
long	count_words(const char *str);
char	**identifier_process_arguments(t_ast_node *args, long *argc,
			t_env *env);

// quote removal

void	free_split(char **split);
char	*remove_quotes(char *str);

// wildcard

bool	wildcard_check_if_matches(char *input, char *pattern);
char	**get_all_matched_names(char *pattern);

// redirections

char	**identifier_process_redirection(t_ast_node *redirection,
			long *argc, t_env *env);

#endif
