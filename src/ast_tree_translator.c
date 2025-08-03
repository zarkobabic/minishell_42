/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_tree_translator.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 14:34:42 by zbabic            #+#    #+#             */
/*   Updated: 2025/08/03 21:07:43 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast_tree_structures.h"
#include <stdbool.h>

//Forward declarations
void	ast_add_identifier_list_to_cmd(t_ast_node *ast_identifier_curr, t_ast_node *command);
t_ast_node	*ast_parse_identifier_list(char *string_value);
void	ast_add_redirection_to_cmd(t_ast_node *redirection, t_ast_node *command);
bool	ast_parse_command_element(t_command_element *element, t_ast_node *ast_cmd);
t_ast_node	*ast_parse_cmd(t_command *command);
t_ast_node	*ast_parse_redirection(t_redirection *redirection);
bool	ast_parse_redirection_list(t_ast_node *ast_subshell,
		t_redirection_list *redirection_list);
t_ast_node	*ast_parse_subshell(t_command *command);
bool	ast_parse_all_commands_for_pipeline(t_ast_node *ast_new_pipeline,
		t_pipeline *pipeline);
t_ast_node	*ast_parse_logical_expr(t_pipeline *pipeline,
		t_logical_expr_type type);
static t_ast_node	*ast_parse_first_logical_expr(t_logical_expr *logical_expr);
t_ast_node	*ast_parse_all_logical_expr(t_logical_expr *logical_expr);
t_ast_node	*ast_create_tree(t_command_line *parse_tree);


void	ast_add_identifier_list_to_cmd(t_ast_node *ast_identifier_curr, t_ast_node *command)
{
	while(ast_identifier_curr)
	{
		if(!command->data.command.executable)
			command->data.command.executable = ast_identifier_curr;
		else
		{
			if(!command->data.command.args)
				command->data.command.args = ast_identifier_curr;
			else
				command->data.command.args_tail->next = ast_identifier_curr;
			command->data.command.args_tail = ast_identifier_curr;
			command->data.command.arg_cnt += 1;
		}
		ast_identifier_curr = ast_identifier_curr->next;
	}
}

//TODO: I need to expand the string here because if the variable has 2 words
//and the first one is a valid command then it should be executable
//Example:
//export helo="echo mama"
//$helo tata
//will become: echo mama tata and be valid to only echo should be written as
//executable, not the whole thing.

t_ast_node	*ast_parse_identifier_list(char *string_value)
{
	t_ast_node	*identifier_node;

	identifier_process_to_char_list(string_value);
	
	identifier_node = ast_create_identifier(string_value);
	return identifier_node;
}

void	ast_add_redirection_to_cmd(t_ast_node *redirection, t_ast_node *command)
{
	if(!command->data.command.redirections)
		command->data.command.redirections = redirection;
	else
		command->data.command.redirections_tail->next = redirection;
	command->data.command.redirections_tail = redirection;
}

bool	ast_parse_command_element(t_command_element *element, t_ast_node *ast_cmd)
{
	t_ast_node	*ast_redirection;
	t_ast_node	*ast_identifier_list;
	
	if(element->type == TYPE_REDIRECTION)
	{
		ast_redirection = ast_parse_redirection(element->command_element.redirection);
		if(!ast_redirection)
			return false;
		element->command_element.redirection->file = NULL;
		ast_add_redirection_to_cmd(ast_redirection, ast_cmd);
		return true;
	}
	else if(element->type == TYPE_TOKEN_IDENTIFIER)
	{
		ast_identifier_list = ast_parse_identifier_list(element->command_element.string_value);
		if(!ast_identifier_list)
			return false;
		element->command_element.string_value = NULL;
		ast_add_identifier_list_to_cmd(ast_identifier_list, ast_cmd);
		return true;
	}
	else
		return false;
}

t_ast_node	*ast_parse_cmd(t_command *command)
{
	t_ast_node	*ast_cmd;
	t_simple_command	*curr_simple_command;

	if(!command)
		return NULL;
	ast_cmd = ast_create_cmd();
	if (!ast_cmd)
		return (NULL);
	curr_simple_command = command->command.simple_command;
	while(curr_simple_command)
	{
		if(!ast_parse_command_element(curr_simple_command->command_element, ast_cmd))
			return (ast_free_command(&ast_cmd), NULL);
		curr_simple_command = curr_simple_command->simple_command;
	}
	return ast_cmd;
}

t_ast_node	*ast_parse_redirection(t_redirection *redirection)
{
	t_ast_node	*ast_redirection;

	if(!redirection)
		return NULL;
	ast_redirection = ast_create_redirection(redirection->type, redirection->file); //TODO: Add extending of the string here, if the string at the end is more than one word report error because in the redirection everything that has space is ambiguous
	if(!ast_redirection)
		return NULL;
	//add_check_for_ambiguous_here
	return ast_redirection;
}

bool	ast_parse_redirection_list(t_ast_node *ast_subshell,
		t_redirection_list *redirection_list)
{
	t_redirection_list	*curr_redirection_list;
	t_ast_node			*ast_parsed_redirection;
	t_ast_node			*ast_last_parsed_redirection;

	if (!redirection_list)
		return (true);
	ast_last_parsed_redirection = NULL;
	curr_redirection_list = redirection_list;
	while (curr_redirection_list)
	{
		ast_parsed_redirection = ast_parse_redirection(
			curr_redirection_list->redirection);
		curr_redirection_list->redirection->file = NULL;
		if (!ast_parsed_redirection)
			return (false);
		if (!ast_last_parsed_redirection)
			ast_subshell->data.subshell.redirections = ast_parsed_redirection;
		else
			ast_last_parsed_redirection->next = ast_parsed_redirection;
		ast_last_parsed_redirection = ast_parsed_redirection;
		curr_redirection_list = curr_redirection_list->next;
	}
	return (true);
}

t_ast_node	*ast_parse_subshell(t_command *command)
{
	t_ast_node	*ast_subshell;
	t_ast_node	*ast_logical_expr;

	ast_subshell = ast_create_subshell();
	if (!ast_subshell)
		return (NULL);
	if (!ast_parse_redirection_list(ast_subshell,
			command->command.composite_command->redirection_list))
		return (ast_free_subshell(&ast_subshell), NULL);
	ast_logical_expr = ast_parse_all_logical_expr(command->command.composite_command->logical_expr);
	if(!ast_logical_expr)
		return (ast_free_subshell(&ast_subshell), NULL);
	ast_subshell->data.subshell.logical_expr = ast_logical_expr;
	return ast_subshell;
}

bool	ast_parse_all_commands_for_pipeline(t_ast_node *ast_new_pipeline,
		t_pipeline *pipeline)
{
	t_pipeline	*curr_pipeline;
	t_ast_node	*ast_curr_command;
	t_ast_node	*ast_last_command;

	ast_last_command = NULL;
	curr_pipeline = pipeline;
	while (curr_pipeline)
	{
		if (curr_pipeline->command->type == TYPE_COMPOSITE_COMMAND)
			ast_curr_command = ast_parse_subshell(curr_pipeline->command);
		else
			ast_curr_command = ast_parse_cmd(curr_pipeline->command);
		if (!ast_curr_command)
			return (false);
		if (!ast_last_command)
			ast_new_pipeline->data.pipeline.cmds = ast_curr_command;
		else
			ast_last_command->next = ast_curr_command;
		ast_new_pipeline->data.pipeline.cmd_cnt += 1;
		ast_last_command = ast_curr_command;
		curr_pipeline = curr_pipeline->pipeline;
	}
	return true;
}

t_ast_node	*ast_parse_logical_expr(t_pipeline *pipeline,
		t_logical_expr_type type)
{
	t_ast_node	*ast_new_logical_expr;
	t_ast_node	*ast_new_pipeline;

	ast_new_logical_expr = ast_create_logical_expr(type);
	if (!ast_new_logical_expr)
		return (NULL);
	ast_new_pipeline = ast_create_pipeline();
	if (!ast_new_pipeline)
		return (ast_free_logical_expr(&ast_new_logical_expr), NULL);
	ast_new_logical_expr->data.logical_expr.pipeline = ast_new_pipeline;
	if (!ast_parse_all_commands_for_pipeline(ast_new_pipeline, pipeline))
		return (ast_free_pipeline(&ast_new_pipeline),
			ast_free_logical_expr(&ast_new_logical_expr), NULL);
	return (ast_new_logical_expr);
}

static t_ast_node	*ast_parse_first_logical_expr(t_logical_expr *logical_expr)
{
	t_logical_expr_type	type;

	type = TYPE_END;
	if (logical_expr->next_logical_expr)
		type = logical_expr->next_logical_expr->op_type;
	return (ast_parse_logical_expr(logical_expr->pipeline, type));
}

t_ast_node	*ast_parse_all_logical_expr(t_logical_expr *logical_expr)
{
	t_ast_node			*ast_logical_expr_head;
	t_ast_node			*ast_curr_logical_expr;
	t_next_logical_expr	*curr_next_logical_expr;
	t_logical_expr_type	type;

	if (!logical_expr)
		return (NULL);
	ast_logical_expr_head = ast_parse_first_logical_expr(logical_expr);
	if (!ast_logical_expr_head)
		return (NULL);
	curr_next_logical_expr = logical_expr->next_logical_expr;
	ast_curr_logical_expr = ast_logical_expr_head;
	while (curr_next_logical_expr)
	{
		type = TYPE_END;
		if (curr_next_logical_expr->next_logical_expr)
			type = curr_next_logical_expr->next_logical_expr->op_type;
		ast_curr_logical_expr->next = ast_parse_logical_expr(curr_next_logical_expr->pipeline,
				type);
		ast_curr_logical_expr = ast_curr_logical_expr->next;
		if (!ast_curr_logical_expr)
			return (ast_free_all_logical_expr(ast_logical_expr_head), NULL);
		curr_next_logical_expr = curr_next_logical_expr->next_logical_expr;
	}
	return (ast_logical_expr_head);
}

t_ast_node	*ast_create_tree(t_command_line *parse_tree)
{
	t_ast_node	*root;

	root = ast_parse_all_logical_expr(parse_tree->logical_expr);
	if (!root)
		return (NULL);
	return root;
}
