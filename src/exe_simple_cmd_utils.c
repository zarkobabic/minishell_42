/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_simple_cmd_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 10:36:31 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/14 11:22:35 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exe_execution.h"
#include "identifier.h"
// #include "exe_redirection.h"

void	free_argv(char ***argv)
{
	long	i;

	i = 0;
	if (!argv || !(*argv))
		return ;
	while ((*argv)[i])
	{
		free((*argv)[i]);
		++i;
	}
	free(*argv);
	*argv = NULL;
}

// Prepare argv for command execution
int	f_prepare_argv(t_ast_node *simcmd, t_cmd_ctx *ctx)
{
	ctx->argc = simcmd->data.command.arg_cnt;
	ctx->argv = identifier_process_arguments(simcmd->data.command.args,
			&ctx->argc, ctx->env);
	if (!ctx->argv && ctx->argc != 0)
		return (perror("argument building failed"), ERROR);
	return (SUCCESS);
}

void	f_create_empty_argv(char ***argv)
{
	*argv = malloc(2 * sizeof(char *));
	(*argv)[0] = malloc(1);
	(*argv)[0][0] = '\0';
	(*argv)[1] = NULL;
}
