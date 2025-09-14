/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_execution_tree_leafs.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 11:03:46 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/14 14:15:23 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exe_execution.h"
#include "exe_redirection.h"

int	f_exec_external(t_cmd_ctx *ctx)
{
	char		*exe_path;
	int			exit_value;

	if (!ctx->argc || !ctx->argv[0])
		return (SUCCESS);
	exit_value = SUCCESS;
	exe_path = resolve_executable_path(ctx->env->minienviron, ctx->argv[0],
			&exit_value);
	if (!exe_path)
		return (exit_value);
	exit_value = execve(exe_path, ctx->argv, ctx->env->minienviron);
	if (exit_value != SUCCESS)
	{
		exit_value = PERMISSION;
		ft_putstr_fd_fail_safe(exe_path, STDERR_FILENO);
		ft_putstr_fd_fail_safe(": Is a directory\n", STDERR_FILENO);
		free(exe_path);
	}
	return (exit_value);
}

int	f_exec_builtin(t_cmd_ctx *ctx)
{
	if (ft_strncmp(ctx->argv[0], "echo", 5) == 0)
		return (f_echo(ctx->argc, ctx->argv));
	else if (ft_strncmp(ctx->argv[0], "cd", 3) == 0)
		return (f_cd(&ctx->env->minienviron, ctx->argc, ctx->argv));
	else if (ft_strncmp(ctx->argv[0], "pwd", 4) == 0)
		return (f_pwd());
	else if (ft_strncmp(ctx->argv[0], "export", 7) == 0)
		return (f_export(&ctx->env->minienviron, ctx->argc, ctx->argv));
	else if (ft_strncmp(ctx->argv[0], "unset", 6) == 0)
		return (f_unset(&ctx->env->minienviron, ctx->argc, ctx->argv));
	else if (ft_strncmp(ctx->argv[0], "env", 4) == 0)
		return (f_env(ctx->env->minienviron, ctx->argc, false));
	else if (ft_strncmp(ctx->argv[0], "exit", 5) == 0)
		return (f_exit(ctx->argc, ctx->argv, &ctx->error));
	else
		return (perror("builtin mapping"), ERROR);
}

// Execute builtin command in parent process
int	f_exec_builtin_parent(t_cmd_ctx *ctx)
{
	int	exit_value;

	if (f_builtin_parent_store_fds(ctx) == ERROR)
		return (ERROR);
	exit_value = SUCCESS;
	exit_value = f_redirect(ctx->redirections, ctx->env);
	if (exit_value == SUCCESS)
		exit_value = f_exec_builtin(ctx);
	if (dup2(ctx->orig_fds[IN], STDIN_FILENO) == -1)
		perror("dup2 restore in");
	if (dup2(ctx->orig_fds[OUT], STDOUT_FILENO) == -1)
		perror("dup2 restore out");
	close(ctx->orig_fds[IN]);
	close(ctx->orig_fds[OUT]);
	if (ft_strncmp(ctx->argv[0], "exit", 5) == 0 && ctx->error == false)
	{
		free_argv(&ctx->argv);
		ast_free_tree(ctx->env->ast_root);
		mini_exit(exit_value, ctx->env);
	}
	return (exit_value);
}
