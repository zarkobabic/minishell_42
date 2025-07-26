/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tree_structures.h                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 15:34:58 by zbabic            #+#    #+#             */
/*   Updated: 2025/07/26 17:17:35 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//////////////////////redirection////////////////

typedef enum e_redirection_type
{
    TYPE_REDIRECT_IN = 3,
    TYPE_REDIRECT_OUT,
    TYPE_REDIRECT_HEREDOC,
    TYPE_REDIRECT_APPEND
} t_redirection_type;

typedef struct s_redirection
{
    t_redirection_type type;
    char *file;
} t_redirection;

//////////////////////redirection_list////////////////

typedef struct s_redirection_list
{
    t_redirection *redirection;
    struct s_redirection_list *next;
} t_redirection_list;

//////////////////////command_element////////////////

typedef enum e_command_element_type
{
    TYPE_TOKEN_IDENTIFIER,
    TYPE_REDIRECTION
} t_command_element_type;

typedef struct s_command_element
{
    t_command_element_type type;
    union u_command_element
    {
        char *string_value;
        t_redirection *redirection;
    } command_element;
} t_command_element;

//////////////////////simple_command////////////////

typedef struct s_simple_command
{
    t_command_element *command_element;
    struct s_simple_command *simple_command;
} t_simple_command;

//////////////////////command////////////////

typedef enum s_command_type
{
    TYPE_SIMPLE_COMMAND,
    TYPE_COMPOSITE_COMMAND
} t_command_type;


typedef struct s_logical_expr t_logical_expr;

typedef struct s_composite_command
{
    t_logical_expr *logical_expr;
    t_redirection_list *redirection_list;
} t_composite_command;

typedef struct s_command
{
    t_command_type type;
    union u_command
    {
        t_simple_command *simple_command;
        t_composite_command *composite_command;
    } command;
} t_command;

//////////////////////pipeline////////////////

typedef struct s_pipeline
{
    t_command *command;
    struct s_pipeline *pipeline;
} t_pipeline;

//////////////////////next_logical_expr////////////////

typedef enum e_logical_expr_type
{
    TYPE_AND = 1,
    TYPE_OR
} t_logical_expr_type;

typedef struct s_next_logical_expr
{
    t_logical_expr_type op_type;
    t_pipeline *pipeline;
    struct s_next_logical_expr *next_logical_expr;
} t_next_logical_expr;

//////////////////////logical_expr////////////////

typedef struct s_logical_expr
{
    t_pipeline *pipeline;
    t_next_logical_expr *next_logical_expr;
} t_logical_expr;

//////////////////////command_line////////////////

typedef struct s_command_line
{
    t_logical_expr *logical_expr;
} t_command_line;
