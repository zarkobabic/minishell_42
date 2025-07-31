/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tree_nodes_constructors_expressions.c        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 03:28:56 by zbabic            #+#    #+#             */
/*   Updated: 2025/07/31 04:41:42 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdlib.h>

t_logical_expr	*create_logical_expr(t_pipeline *pipeline,
		t_next_logical_expr *next)
{
	t_logical_expr	*expr;

	expr = malloc(sizeof(t_logical_expr));
	if (!expr)
		return (NULL);
	expr->pipeline = pipeline;
	expr->next_logical_expr = next;
	return (expr);
}

t_next_logical_expr	*create_next_logical_expr(t_logical_expr_type op_type,
		t_pipeline *pipeline, t_next_logical_expr *next)
{
	t_next_logical_expr	*expr;

	expr = malloc(sizeof(t_next_logical_expr));
	if (!expr)
		return (NULL);
	expr->op_type = op_type;
	expr->pipeline = pipeline;
	expr->next_logical_expr = next;
	return (expr);
}

t_pipeline	*create_pipeline(t_command *command, t_pipeline *next)
{
	t_pipeline	*pipeline;

	pipeline = malloc(sizeof(t_pipeline));
	if (!pipeline)
		return (NULL);
	pipeline->command = command;
	pipeline->pipeline = next;
	return (pipeline);
}

t_command_line	*create_command_line(t_logical_expr *logical_expr)
{
	t_command_line	*cmd_line;

	cmd_line = malloc(sizeof(t_command_line));
	if (!cmd_line)
		return (NULL);
	cmd_line->logical_expr = logical_expr;
	return (cmd_line);
}
