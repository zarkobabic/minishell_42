/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_tree_print.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 19:29:13 by zbabic            #+#    #+#             */
/*   Updated: 2025/08/05 18:32:55 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast_tree_structures.h"
#include <stdio.h>

#define NESTING_PADDING 4

static void	print_padding(int padding)
{
	int	counter;

	counter = 0;
	while (counter < padding)
	{
		printf(" ");
		++counter;
	}
}

static char	*ast_get_redirection_type_string(t_redirection_type type)
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

static char	*ast_get_log_expr_op_string(t_logical_expr_type type)
{
	if (type == TYPE_AND)
		return ("TYPE_AND(&&)");
	else if (type == TYPE_OR)
		return ("TYPE_OR(||)");
	else if (type == TYPE_END)
		return ("TYPE_END(NULL)");
	else
		return ("ERROR!");
}

void	ast_print_identifier(t_ast_node *identifier, int padding)
{
	print_padding(padding);
	if (identifier->type != AST_IDENTIFIER)
	{
		printf("ERROR!\n");
		return ;
	}
	printf("%s\n", identifier->data.identifier.string_value);
}

void	ast_print_redirection(t_ast_node *redirection, int padding)
{
	print_padding(padding);
	if (redirection->type != AST_REDIRECTION)
	{
		printf("ERROR!\n");
		return ;
	}
	printf("REDIRECTION: type: %s, target: %s\n",
		ast_get_redirection_type_string(redirection->data.redirection.type),
		redirection->data.redirection.target);
}

void	ast_print_simple_command(t_ast_node *command, int padding)
{
	t_ast_node	*current_el;

	printf("COMMAND: arg_cnt: %ld\n", command->data.command.arg_cnt);
	print_padding(padding + NESTING_PADDING);
	printf("executable:\n");
	ast_print_identifier(command->data.command.executable, padding + 2
		* NESTING_PADDING);
	print_padding(padding + NESTING_PADDING);
	printf("arguments:\n");
	current_el = command->data.command.args;
	while (current_el)
	{
		ast_print_identifier(current_el, padding + 2 * NESTING_PADDING);
		current_el = current_el->next;
	}
	print_padding(padding + NESTING_PADDING);
	printf("redirections:\n");
	current_el = command->data.command.redirections;
	while (current_el)
	{
		ast_print_redirection(current_el, padding + 2 * NESTING_PADDING);
		current_el = current_el->next;
	}
}

void		ast_print_logical_expr(t_ast_node *logical_expr, int padding);

void	ast_print_subshell(t_ast_node *subshell, int padding)
{
	t_ast_node	*curr_redirection;

	printf("SUBSHELL:\n");
	print_padding(padding + NESTING_PADDING);
	printf("redirections:\n");
	curr_redirection = subshell->data.subshell.redirections;
	while (curr_redirection)
	{
		ast_print_redirection(curr_redirection, padding + 2 * NESTING_PADDING);
		curr_redirection = curr_redirection->next;
	}
	ast_print_logical_expr(subshell->data.subshell.logical_expr, padding
		+ NESTING_PADDING);
}

void	ast_print_command(t_ast_node *command, int padding)
{
	print_padding(padding);
	if (!(command->type == AST_CMD || command->type == AST_SUBSHELL))
	{
		printf("ERROR!\n");
		return ;
	}
	if (command->type == AST_CMD)
		ast_print_simple_command(command, padding);
	else if (command->type == AST_SUBSHELL)
		ast_print_subshell(command, padding);
	else
		printf("ERROR!\n");
}

void	ast_print_pipeline(t_ast_node *pipeline, int padding)
{
	t_ast_node	*curr_cmd;

	print_padding(padding);
	if (pipeline->type != AST_PIPELINE)
	{
		printf("ERROR!\n");
		return ;
	}
	printf("PIPELINE: cmd_count: %ld\n", pipeline->data.pipeline.cmd_cnt);
	curr_cmd = pipeline->data.pipeline.cmds;
	while (curr_cmd)
	{
		ast_print_command(curr_cmd, padding + NESTING_PADDING);
		curr_cmd = curr_cmd->next;
	}
}

void	ast_print_logical_expr(t_ast_node *logical_expr, int padding)
{
	if (!logical_expr)
		return ;
	print_padding(padding);
	if (logical_expr->type != AST_LOGICAL_EXPR)
	{
		printf("ERROR!\n");
		return ;
	}
	printf("LOGICAL_EXPR: op_type: %s\n",
		ast_get_log_expr_op_string(logical_expr->data.logical_expr.type));
	ast_print_pipeline(logical_expr->data.logical_expr.pipeline, padding
		+ NESTING_PADDING);
	if (logical_expr->next)
		ast_print_logical_expr(logical_expr->next, padding);
}

void	ast_tree_print(t_ast_node *root)
{
	if (!root)
		printf("AST_TREE: NOTHING_TO_SHOW!\n");
	else
		printf("AST_TREE:\n");
	if (root->type == AST_REDIRECTION)
		ast_print_redirection(root, NESTING_PADDING);
	else if (root->type == AST_CMD)
		ast_print_command(root, NESTING_PADDING);
	else if (root->type == AST_SUBSHELL)
		ast_print_subshell(root, NESTING_PADDING);
	else if (root->type == AST_PIPELINE)
		ast_print_pipeline(root, NESTING_PADDING);
	else if (root->type == AST_LOGICAL_EXPR)
		ast_print_logical_expr(root, NESTING_PADDING);
	else if (root->type == AST_IDENTIFIER)
		ast_print_identifier(root, NESTING_PADDING);
}
