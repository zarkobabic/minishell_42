/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_build_heredoc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 08:52:50 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/13 09:10:32 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exe_execution.h"

void	f_build_heredoc_logical_expr(t_ast_node *ast_logical_expr, t_env *env);

void	f_build_heredoc_subshell(t_ast_node *subshell, t_env *env)
{
	//children
	f_build_heredoc_logical_expr(subshell->data.subshell.logical_expr, env);
	//it
	f_setup_heredocs(subshell, env);
}

void	f_build_heredoc_pipeline(t_ast_node *pipeline, t_env *env)
{
	t_ast_node	*cmds;

	cmds = pipeline->data.pipeline.cmds;
	while(cmds)
	{
		if(cmds->type == AST_CMD)
			f_setup_heredocs(cmds, env);
		else if(cmds->type == AST_SUBSHELL)
			f_build_heredoc_subshell(cmds, env);
		cmds=cmds->next;
	}
}

void	f_build_heredoc_logical_expr(t_ast_node *ast_logical_expr, t_env *env)
{
	if(ast_logical_expr->type != AST_LOGICAL_EXPR)
		return ;
	f_build_heredoc_pipeline(ast_logical_expr->data.logical_expr.pipeline, env);
	if(ast_logical_expr->next)
		f_build_heredoc_logical_expr(ast_logical_expr->next, env);
}