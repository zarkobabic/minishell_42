/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draft.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 22:13:45 by eberkau           #+#    #+#             */
/*   Updated: 2025/08/23 23:37:09 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"
#include "../src/ast_tree_structures.h"

int	f_execute_cmd(t_ast_node *cmd)
{
	// filter for builtins
	if (cmd->type == AST_IDENTIFIER
		&& (cmd->data.identifier.string_value == "echo"
		|| cmd->data.identifier.string_value == "cd"
		|| cmd->data.identifier.string_value == "pwd"
		|| cmd->data.identifier.string_value == "export"
		|| cmd->data.identifier.string_value == "unset"
		|| cmd->data.identifier.string_value == "env"
		|| cmd->data.identifier.string_value == "exit"))
		f_execute_builtin(cmd);
	else
	// fork and so on
}

int	f_exec_subshell(t_ast_node *subshell)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		f_exec_logic_expr();
	}
	else
		waitpid;
	return ();
}

int f_exec_simple_cmd(t_ast_node *simcmd)
{
	char	**argv;
	long	argc;
	
	argv = f_buildargv(simcmd->data.command.args);
	argc = simcmd->data.command.arg_cnt;
}

// int	f_execute_cmd(t_ast_node *cmd)
// {
// 	pid_t	pid;

// 	// filter for builtins
// 	if (cmd->type == AST_IDENTIFIER
// 		&& (cmd->data.identifier.string_value == "echo"
// 		|| cmd->data.identifier.string_value == "cd"
// 		|| cmd->data.identifier.string_value == "pwd"
// 		|| cmd->data.identifier.string_value == "export"
// 		|| cmd->data.identifier.string_value == "unset"
// 		|| cmd->data.identifier.string_value == "env"
// 		|| cmd->data.identifier.string_value == "exit"))
// 		f_execute_builtin(cmd);
// 	else
// 	pid = fork();
// 	if (pid == 0)
// 	{
// 		if (cmd->type == AST_CMD)
// 	}
// }


int	f_exec_cmd(t_ast_node *cmd)
{
	if (cmd->type == AST_CMD)
		f_exec_simple_cmd();
	else if (cmd->type == AST_SUBSHELL)
		f_exec_subshell();
	
}

int	f_exec_pipeline(t_ast_node *pipeline)
{
	
}

int	f_exec_logic_expr(t_ast_node *expr)
{
	int	exit_value;

	if (!expr || expr->type != AST_LOGICAL_EXPR)
		return (ERROR);
	exit_value = f_exec_pipeline(expr->data.logical_expr.pipeline);
	if ((expr->data.logical_expr.type == TYPE_AND
		&& exit_value == SUCCESS)
		|| (expr->data.logical_expr.type == TYPE_OR
		&& exit_value == FAIL))
		exit_value = f_exec_logic_expr(expr->next);
	return (exit_value);
}

int	f_execute_ast(t_ast_node *root)
{
	int	exit_value;

	if (!root || root->type != AST_LOGICAL_EXPR)
		return (ERROR);
	exit_value = f_exec_logic_expr(root);
	return (exit_value);
}
