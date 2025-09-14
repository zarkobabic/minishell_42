/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_redirection_heredoc_build_array.c              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 08:52:50 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/14 11:18:34 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exe_redirection.h"

int	f_heredoc_build_logical_expr(t_ast_node *ast_logical_expr, t_env *env);

static int	f_heredoc_setup_files(t_ast_node *cmd, t_env *env)
{
	static int	hrdc_counter = 0;
	t_ast_node	*temp_redir;

	if (!cmd)
		return (GENERAL_ERR);
	hrdc_counter = hrdc_counter % HEREDOC_COUNT_MAX;
	if (cmd->type == AST_SUBSHELL)
		temp_redir = cmd->data.subshell.redirections;
	else
		temp_redir = cmd->data.command.redirections;
	while (temp_redir)
	{
		if (temp_redir->data.redirection.type == TYPE_REDIRECT_HEREDOC)
		{
			env->heredoclist[hrdc_counter].redir_ptr = temp_redir;
			f_heredoc_populate_entry_file_name(env, hrdc_counter);
			if (f_heredoc_read_input(env, hrdc_counter,
					temp_redir->data.redirection.target) != SUCCESS)
				return (GENERAL_ERR);
			++hrdc_counter;
		}
		temp_redir = temp_redir->next;
	}
	return (SUCCESS);
}

static int	f_heredoc_build_subshell(t_ast_node *subshell, t_env *env)
{
	int	exit_value;

	exit_value = f_heredoc_build_logical_expr(
			subshell->data.subshell.logical_expr, env);
	if (exit_value != SUCCESS)
		return (exit_value);
	exit_value = f_heredoc_setup_files(subshell, env);
	return (exit_value);
}

static int	f_heredoc_build_pipeline(t_ast_node *pipeline, t_env *env)
{
	t_ast_node	*cmds;
	int			exit_value;

	cmds = pipeline->data.pipeline.cmds;
	while (cmds)
	{
		if (cmds->type == AST_CMD)
			exit_value = f_heredoc_setup_files(cmds, env);
		else if (cmds->type == AST_SUBSHELL)
			exit_value = f_heredoc_build_subshell(cmds, env);
		if (exit_value != SUCCESS)
			break ;
		cmds = cmds->next;
	}
	return (exit_value);
}

int	f_heredoc_build_logical_expr(t_ast_node *ast_logical_expr, t_env *env)
{
	int	exit_value;

	if (ast_logical_expr->type != AST_LOGICAL_EXPR)
		return (GENERAL_ERR);
	exit_value = f_heredoc_build_pipeline(
			ast_logical_expr->data.logical_expr.pipeline, env);
	if (exit_value != SUCCESS)
		return (exit_value);
	if (ast_logical_expr->next)
		exit_value = f_heredoc_build_logical_expr(ast_logical_expr->next, env);
	return (exit_value);
}
