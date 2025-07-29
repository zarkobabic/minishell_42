/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tree_nodes_destructors_operators.c           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 12:41:41 by zbabic            #+#    #+#             */
/*   Updated: 2025/07/29 15:36:32 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdlib.h>

void	free_pipeline(t_pipeline *pipeline)
{
	if (!pipeline)
		return;
	if (pipeline->command)
		free_command(pipeline->command);
	if (pipeline->pipeline)
		free_pipeline(pipeline->pipeline);
	free(pipeline);
}

void	free_next_logical_expr(t_next_logical_expr *next_logical_expr)
{
	if (!next_logical_expr)
		return;
	if (next_logical_expr->pipeline)
		free_pipeline(next_logical_expr->pipeline);
	if (next_logical_expr->next_logical_expr)
		free_next_logical_expr(next_logical_expr->next_logical_expr);
	free(next_logical_expr);
}

void	free_logical_expr(t_logical_expr *logical_expr)
{
	if (!logical_expr)
		return;
	if (logical_expr->pipeline)
		free_pipeline(logical_expr->pipeline);
	if (logical_expr->next_logical_expr)
		free_next_logical_expr(logical_expr->next_logical_expr);
	free(logical_expr);
}

void	free_command_line(t_command_line *cmd_line)
{
	if (!cmd_line)
		return;
	if (cmd_line->logical_expr)
		free_logical_expr(cmd_line->logical_expr);
	free(cmd_line);
}

void free_parse_tree(t_command_line *parse_tree)
{
    free_command_line(parse_tree);
}
