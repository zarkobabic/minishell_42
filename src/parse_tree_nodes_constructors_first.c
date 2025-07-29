/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tree_nodes_constructors_first.c              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 03:28:56 by zbabic            #+#    #+#             */
/*   Updated: 2025/07/29 17:54:07 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdlib.h>

t_logical_expr	*create_logical_expr(t_pipeline *pipeline, t_next_logical_expr *next)
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

t_command	*create_simple_command_node(t_simple_command *simple_command)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->type = TYPE_SIMPLE_COMMAND;
	cmd->command.simple_command = simple_command;
	return (cmd);
}

t_command	*create_composite_cmd(t_logical_expr *logical_expr,
		t_redirection_list *redirection_list)
{
	t_command			*cmd;
	t_composite_command	*composite;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	composite = malloc(sizeof(t_composite_command));
	if (!composite)
	{
		free(cmd);
		return (NULL);
	}
	composite->logical_expr = logical_expr;
	composite->redirection_list = redirection_list;
	cmd->type = TYPE_COMPOSITE_COMMAND;
	cmd->command.composite_command = composite;
	return (cmd);
}
