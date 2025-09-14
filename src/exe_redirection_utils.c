/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_redirection_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 10:14:54 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/14 11:12:54 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include <unistd.h>
// #include "exe_redirection.h"
#include "identifier.h"

// static int	f_check_ambiguity(char *target)
// {
// 	char	**target_list;
// 	int		i;

// 	i = -1;
// 	while (target[++i])
// 	{
// 		if (target[i] == ' ')
// 			return (true);
// 	}
// 	target_list = get_all_matched_names(target);
// 	if (!target_list)
// 		return (ERROR);
// 	if (target_list[0] && target_list[1])
// 		return (true);
// 	i = 0;
// 	while (target_list[i])
// 		free(target_list[i++]);
// 	free(target_list);
// 	return (false);
// }

char	**f_expand_vars(t_ast_node *redirection, t_env *env)
{
	long	arbitrary_argc;
	char	**expanded_result;

	expanded_result = identifier_process_redirection(redirection,
			&arbitrary_argc, env);
	if (!expanded_result)
		return (NULL);
	if (arbitrary_argc > 1)
		return (free_split(expanded_result),
			ft_putstr_fd(redirection->data.redirection.target,
				STDERR_FILENO),
			ft_putstr_fd(": ambiguous redirection\n", STDERR_FILENO),
			NULL);
	else
		return (expanded_result);
}

char	*f_heredoc_find_entry(t_ast_node *target, t_env *env)
{
	int	i;

	i = 0;
	while (i < HEREDOC_COUNT_MAX)
	{
		if (env->heredoclist[i].redir_ptr == target)
			return (env->heredoclist[i].name);
		++i;
	}
	return (NULL);
}
