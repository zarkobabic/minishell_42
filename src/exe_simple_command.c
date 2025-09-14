/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_simple_command.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 19:04:18 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/14 05:35:19 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exe_execution.h"
#include "exe_redirection.h"

void	free_argv(char ***argv)
{
	long	i;

	i = 0;
	if(!argv || !(*argv))
		return ;
	while((*argv)[i])
	{
		free((*argv)[i]);
		++i;
	}
	free(*argv);
	*argv = NULL;
}

int	f_exec_external(t_cmd_ctx *ctx)
{
	char		*exe_path;
	int			exit_value;

	if (!ctx->argc || !ctx->argv[0])
		return (SUCCESS);
	exit_value = SUCCESS;
	exe_path = resolve_executable_path(ctx->env->minienviron, ctx->argv[0], &exit_value);
	if (!exe_path)
		return (exit_value);
	exit_value = execve(exe_path, ctx->argv, ctx->env->minienviron);
	if (exit_value != SUCCESS)
	{
		exit_value = PERMISSION;
		free(exe_path);
		ft_putstr_fd_fail_safe(exe_path, STDERR_FILENO);
		ft_putstr_fd_fail_safe(": Is a directory\n", STDERR_FILENO);
	}
	return (exit_value);
}

// Prepare argv for command execution
static int	f_prepare_argv(t_ast_node *simcmd, t_cmd_ctx *ctx)
{
	ctx->argc = simcmd->data.command.arg_cnt;
	ctx->argv = identifier_process_arguments(simcmd->data.command.args, &ctx->argc, ctx->env);
	if (!ctx->argv && ctx->argc != 0)
		return (perror("argument building failed"), ERROR);
	return (SUCCESS);
}

// Setup file descriptors for builtin parent execution
static int	f_setup_builtin_parent_fds(t_cmd_ctx *ctx)
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

// Execute builtin command in parent process
static int	f_exec_builtin_parent(t_cmd_ctx *ctx)
{
	int	exit_value;

	if (f_setup_builtin_parent_fds(ctx) == ERROR)
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

// Handle child process execution
static int	f_handle_child_process(t_cmd_ctx *ctx)
{
	int	exit_value;

	exit_value = SUCCESS;
	exit_value = f_redirect(ctx->redirections, ctx->env);
	if (exit_value == SUCCESS)
	{
		if (f_is_builtin_child(ctx->argv[0]))
			exit_value = f_exec_builtin(ctx);
		else
			exit_value = f_exec_external(ctx);
	}
	f_cleanup_child(ctx->env->ast_root, ctx->argv);
	mini_exit(exit_value, ctx->env);
	return (exit_value);
}

// Execute external command
static int	f_fork_cmd(t_cmd_ctx *ctx)
{
	pid_t	pid;
	int		exit_value;

	pid = fork();
	if (pid == 0)
		return (f_handle_child_process(ctx));
	else if (pid > 0)
		exit_value = f_wait_for_child(pid);
	else
	{
		free_argv(&ctx->argv);
		return (perror("fork"), ERROR);
	}
	return (exit_value);
}

// Execute command in pipeline
static int	f_exec_in_pipeline(t_cmd_ctx *ctx)
{
	int	exit_value;

	exit_value = f_redirect(ctx->redirections, ctx->env);
	if (exit_value != SUCCESS)
		return (exit_value);
	if (f_is_builtin(ctx->argv[0]))
		exit_value = f_exec_builtin(ctx);
	else
		exit_value = f_exec_external(ctx);
	return (exit_value);
}

void	create_empty_argv(char ***argv)
{
	*argv = malloc(2 * sizeof(char*));
	(*argv)[0] = malloc(1);
	(*argv)[0][0] = '\0';
	(*argv)[1] = NULL;
}

// Main simple command execution function
int	f_exec_simple_cmd(t_ast_node *simcmd, t_env *env, int in_pipeline)
{
	t_cmd_ctx	ctx;
	int			exit_value;

	// if (simcmd->data.command.arg_cnt <= 0 || !simcmd->data.command.args)
	// 	return (SUCCESS);
	ctx.env = env;
	ctx.error = false;
	ctx.redirections = simcmd->data.command.redirections;
	ctx.in_pipeline = in_pipeline;
	if (f_prepare_argv(simcmd, &ctx) == ERROR)
		return (ERROR);
	if(!ctx.argv)
		create_empty_argv(&ctx.argv);
	if (!in_pipeline && f_is_builtin_parent(ctx.argv[0]))
		exit_value = f_exec_builtin_parent(&ctx);
	else if (!in_pipeline)
		exit_value = f_fork_cmd(&ctx);
	else
		exit_value = f_exec_in_pipeline(&ctx);
	free_argv(&ctx.argv);
	return (exit_value);
}
