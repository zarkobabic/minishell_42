/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_tree_nodes_constructors_commands.c             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 18:30:34 by zbabic            #+#    #+#             */
/*   Updated: 2025/09/14 14:31:52 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast_tree_translator.h"

t_ast_node	*ast_create_cmd(void)
{
	t_ast_node	*node;

	node = malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	ft_memset(node, 0, sizeof(t_ast_node));
	node->type = AST_CMD;
	return (node);
}

t_ast_node	*ast_create_subshell(void)
{
	t_ast_node	*node;

	node = malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	ft_memset(node, 0, sizeof(t_ast_node));
	node->type = AST_SUBSHELL;
	return (node);
}

t_ast_node	*ast_create_redirection(t_redirection_type type, char *target)
{
	t_ast_node	*node;

	node = malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	ft_memset(node, 0, sizeof(t_ast_node));
	node->type = AST_REDIRECTION;
	node->data.redirection.target = target;
	node->data.redirection.type = type;
	return (node);
}

t_ast_node	*ast_create_identifier(char *value)
{
	t_ast_node	*node;

	node = malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	ft_memset(node, 0, sizeof(t_ast_node));
	node->type = AST_IDENTIFIER;
	node->data.identifier.string_value = value;
	return (node);
}
