/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_execution_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 09:38:59 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/14 12:13:06 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exe_execution.h"
#include "exe_redirection.h"
#include <sys/wait.h>
#include <readline/history.h>

void	mini_exit(int exit_status, t_env *env)
{
	f_environ_destroy(&env->minienviron, ENV_DELETE_ALL);
	clear_history();
	exit(exit_status);
}

bool	f_is_builtin_parent(char *call)
{
	return (!ft_strncmp(call, "cd", 3) || !ft_strncmp(call, "export", 7)
		|| !ft_strncmp(call, "unset", 6) || !ft_strncmp(call, "exit", 5));
}

bool	f_is_builtin_child(char *call)
{
	return (!ft_strncmp(call, "echo", 5) || !ft_strncmp(call, "pwd", 4)
		|| !ft_strncmp(call, "env", 4));
}

bool	f_is_builtin(char *call)
{
	return (f_is_builtin_parent(call) || f_is_builtin_child(call));
}

// Setup file descriptors for builtin parent execution
int	f_builtin_parent_store_fds(t_cmd_ctx *ctx)
{
	ctx->orig_fds[IN] = dup(STDIN_FILENO);
	ctx->orig_fds[OUT] = dup(STDOUT_FILENO);
	if (ctx->orig_fds[IN] == -1 || ctx->orig_fds[OUT] == -1)
	{
		if (ctx->orig_fds[IN] != -1)
			close(ctx->orig_fds[IN]);
		if (ctx->orig_fds[OUT] != -1)
			close(ctx->orig_fds[OUT]);
		free_argv(&ctx->argv);
		return (perror("dup"), ERROR);
	}
	return (SUCCESS);
}
