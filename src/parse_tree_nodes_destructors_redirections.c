/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tree_nodes_destructors_redirections.c        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 13:14:42 by zbabic            #+#    #+#             */
/*   Updated: 2025/07/29 15:36:36 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdlib.h>

void	free_redirection(t_redirection *redirection)
{
	if (!redirection)
		return;
	if (redirection->file)
		free(redirection->file);
	free(redirection);
}

void	free_redirection_list(t_redirection_list *redirection_list)
{
	if (!redirection_list)
		return;
	if (redirection_list->redirection)
		free_redirection(redirection_list->redirection);
	if (redirection_list->next)
		free_redirection_list(redirection_list->next);
	free(redirection_list);
}
