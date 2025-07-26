/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tree_nodes_constructors_third.c              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 03:36:25 by zbabic            #+#    #+#             */
/*   Updated: 2025/07/26 04:09:11 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdlib.h>

t_command_line *create_command_line(t_logical_expr *logical_expr)
{
    t_command_line *cmd_line = malloc(sizeof(t_command_line));
    if (!cmd_line)
        return NULL;
    
    cmd_line->logical_expr = logical_expr;
    return cmd_line;
}
