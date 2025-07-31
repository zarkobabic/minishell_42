/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tree_print.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 14:31:18 by zbabic            #+#    #+#             */
/*   Updated: 2025/07/31 03:54:16 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_tree_structures.h"
#include <stdio.h>

#define NESTING_PADDING 4

// Forward declaration that is requiresd by command
void	parse_tree_print_logical_expr(t_logical_expr *logical_expr,
			int padding);

void	print_padding(int padding)
{
	int	counter;

	counter = 0;
	while (counter < padding)
	{
		printf(" ");
		++counter;
	}
}

char	*parse_tree_get_redirection_type_string(t_redirection_type type)
{
	if (type == TYPE_REDIRECT_IN)
		return ("TYPE_REDIRECT_IN(<)");
	else if (type == TYPE_REDIRECT_OUT)
		return ("TYPE_REDIRECT_OUT(>)");
	else if (type == TYPE_REDIRECT_APPEND)
		return ("TYPE_REDIRECT_APPEND(>>)");
	else if (type == TYPE_REDIRECT_HEREDOC)
		return ("TYPE_REDIRECT_HEREDOC(<<)");
	else
		return ("ERROR!");
}

char	*parse_tree_get_log_expr_op_string(t_logical_expr_type type)
{
	if (type == TYPE_AND)
		return ("TYPE_AND(&&)");
	else if (type == TYPE_OR)
		return ("TYPE_OR(||)");
	else
		return ("ERROR!");
}

void	parse_tree_print_redirection(t_redirection *redirection, int padding)
{
	print_padding(padding);
	if (!redirection)
	{
		printf("REDIRECTION: NULL\n");
		return ;
	}
	printf("REDIRECTION: type: %s, file: %s\n",
		parse_tree_get_redirection_type_string(redirection->type),
		redirection->file);
}

void	parse_tree_print_redirection_list(t_redirection_list *redirection_list,
		int padding)
{
	print_padding(padding);
	if (!redirection_list)
	{
		printf("REDIRECTION_LIST: NULL\n");
		return ;
	}
	printf("REDIRECTION_LIST:\n");
	parse_tree_print_redirection(redirection_list->redirection, padding
		+ NESTING_PADDING);
	parse_tree_print_redirection_list(redirection_list->next, padding
		+ NESTING_PADDING);
}

void	parse_tree_print_command_element(t_command_element *cmd_element,
		int padding)
{
	print_padding(padding);
	if (!cmd_element)
	{
		printf("COMMAND_ELEMENT: NULL\n");
		return ;
	}
	else if (cmd_element->type == TYPE_TOKEN_IDENTIFIER)
		printf("COMMAND_ELEMENT: type: IDENTIFIER, value: %s\n",
			cmd_element->command_element.string_value);
	else if (cmd_element->type == TYPE_REDIRECTION)
	{
		printf("COMMAND_ELEMENT: type: REDIRECTION\n");
		parse_tree_print_redirection(cmd_element->command_element.redirection,
			padding + NESTING_PADDING);
	}
	else
		printf("ERROR!");
}

void	parse_tree_print_simple_command(t_simple_command *simple_command,
		int padding)
{
	print_padding(padding);
	if (!simple_command)
	{
		printf("SIMPLE_COMMAND: NULL\n");
		return ;
	}
	printf("SIMPLE_COMMAND:\n");
	parse_tree_print_command_element(simple_command->command_element, padding
		+ NESTING_PADDING);
	parse_tree_print_simple_command(simple_command->simple_command, padding
		+ NESTING_PADDING);
}

void	parse_tree_print_command(t_command *command, int padding)
{
	print_padding(padding);
	if (!command)
	{
		printf("COMMAND: NULL\n");
		return ;
	}
	else if (command->type == TYPE_SIMPLE_COMMAND)
	{
		printf("COMMAND: type: simple_command\n");
		parse_tree_print_simple_command(command->command.simple_command, padding
			+ NESTING_PADDING);
	}
	else if (command->type == TYPE_COMPOSITE_COMMAND)
	{
		printf("COMMAND: type: composite_command\n");
		parse_tree_print_logical_expr(
			command->command.composite_command->logical_expr,
			padding + NESTING_PADDING);
		parse_tree_print_redirection_list(
			command->command.composite_command->redirection_list,
			padding + NESTING_PADDING);
	}
	else
		printf("ERROR!");
}

void	parse_tree_print_pipeline(t_pipeline *pipeline, int padding)
{
	print_padding(padding);
	if (!pipeline)
	{
		printf("PIPELINE: NULL\n");
		return ;
	}
	printf("PIPELINE:\n");
	parse_tree_print_command(pipeline->command, padding + NESTING_PADDING);
	parse_tree_print_pipeline(pipeline->pipeline, padding + NESTING_PADDING);
}

void	parse_tree_print_next_logical_expr(
	t_next_logical_expr *next_logical_expr, int padding)
{
	print_padding(padding);
	if (!next_logical_expr)
	{
		printf("NEXT_LOGICAL_EXPR: NULL\n");
		return ;
	}
	printf("NEXT_LOGICAL_EXPR: op_type %s\n",
		parse_tree_get_log_expr_op_string(next_logical_expr->op_type));
	parse_tree_print_pipeline(next_logical_expr->pipeline, padding
		+ NESTING_PADDING);
	parse_tree_print_next_logical_expr(next_logical_expr->next_logical_expr,
		padding + NESTING_PADDING);
}

void	parse_tree_print_logical_expr(t_logical_expr *logical_expr, int padding)
{
	print_padding(padding);
	if (!logical_expr)
	{
		printf("LOGICAL_EXPR: NULL\n");
		return ;
	}
	printf("LOGICAL_EXPR:\n");
	parse_tree_print_pipeline(logical_expr->pipeline, padding
		+ NESTING_PADDING);
	parse_tree_print_next_logical_expr(logical_expr->next_logical_expr, padding
		+ NESTING_PADDING);
}

void	parse_tree_print_command_line(t_command_line *command_line, int padding)
{
	print_padding(padding);
	if (!command_line)
	{
		printf("COMMAND_LINE: NULL\n");
		return ;
	}
	printf("COMMAND_LINE:\n");
	parse_tree_print_logical_expr(command_line->logical_expr, padding
		+ NESTING_PADDING);
}

void	parse_tree_print(t_command_line *command_line)
{
	printf("PARSE TREE (Each child is nested by padding in the next line):\n");
	parse_tree_print_command_line(command_line, 0);
}
