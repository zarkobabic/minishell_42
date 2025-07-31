/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_tree_structures.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 18:58:15 by zbabic            #+#    #+#             */
/*   Updated: 2025/07/30 19:07:14 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_TREE_STRUCTURES_H
# define AST_TREE_STRUCTURES_H

typedef enum e_ast_node_type
{
	AST_CMD,
	AST_REDIRECTION,
	AST_LOGICAL_EXPR,
	AST_PIPELINE,
	AST_SUBSHELL
}	t_ast_node_type;

#endif