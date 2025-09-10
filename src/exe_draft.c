/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_draft.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 22:13:45 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/10 22:09:06 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exe_execution.h"
#include <sys/wait.h>

int		f_exec_logic_expr(t_ast_node *expr, t_env *env);

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

void	f_cleanup_child(t_ast_node *root, char **argv)
{
	ast_free_tree(root);
	if (argv)
		free_argv(&argv);
		//free(argv);
}

int	f_wait_for_child(pid_t pid)
{
	int	status;
	int	exit_value;

	if (waitpid(pid, &status, 0) < 0)
		return (ERROR);
	if (WIFEXITED(status))
		exit_value = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		exit_value = 128 + WTERMSIG(status);
	else
		exit_value = ERROR;
	return (exit_value);
}

char** identifier_process_arguments(t_ast_node *args, long *argc, t_env *env)
{
	char	*expanded_string;
	t_arg_list	arg_list;
	char	**splitted_string;
	long	i;
	long	j;
	char	**temp_matched;
	char	*temp;
	char	**result_array;

	arg_list_initialize(&arg_list);
	while(args) //expansion for each t_ast_node args in args linked list
	{
		expanded_string = identifier_expand_variables_in_string(args->data.identifier.string_value, env);
		if(!expanded_string)
			return NULL;
		splitted_string = split_by_unquoted_space(expanded_string);
		free(expanded_string);
		i = 0;
		while (splitted_string[i])
		{
			j = 0;
			temp_matched = get_all_matched_names(splitted_string[i]); //TODO: check if it fails
			//TODO: add cleanup if temp_matched fails
			while(temp_matched[j])
			{
				temp = remove_quotes(temp_matched[j]); //TODO: check if it fails
				arg_list_append(&arg_list, temp);
				++j;
			}
			free(temp_matched);
			free(splitted_string[i]);
			++i;
		}
		free(splitted_string);
		args = args->next;
	}
	result_array = arg_list_to_array_shallow(&arg_list);
	*argc = arg_list.size;
	arg_list_free_shallow(&arg_list);
	return result_array;
}

// TODO: ADD VARIABLE EXPANSION THIS IS FOR $?
// FIXME: check when old_exit_value needs to be expanded and is maybe not needed anymore
char	**f_buildargv(t_ast_node *args, long *argc, t_env *env, bool *empty_string)
{
	char **argv;
	// long i;

	if (!args || args->type != AST_IDENTIFIER)
		return (NULL);
	
	argv = identifier_process_arguments(args, argc, env);
	if(*argc == 0)
		*empty_string = true;
	// if (!argv)
	// 	return (NULL);
	// i = 0;
	// while (args && i < *argc)
	// {
	// 	argv[i] = args->data.identifier.string_value;
	// 	args = args->next;
	// 	++i;
	// }
	// argv[i] = NULL;
	// if (args || i != *argc)
	// 	return (free(argv), NULL);
	return (argv);
}

int	f_exec_subshell(t_ast_node *subshell, t_env *env)
{
	pid_t	pid;
	int		exit_value;

	pid = fork();
	if (pid == 0)
	{
		exit_value = SUCCESS;
		// exit_value = f_redirect(subshell->data.subshell.redirections);
		if (exit_value == SUCCESS)
			exit_value = f_exec_logic_expr(subshell->data.subshell.logical_expr, env);
		ast_free_tree(env->ast_root);
		mini_exit(exit_value, env);
	}
	else if (pid > 0)
		exit_value = f_wait_for_child(pid);
	else
		return (perror("fork"), ast_free_tree(env->ast_root), ERROR);
	return (exit_value);
}

int	f_exec_cmd(t_ast_node *cmd, t_env *env, bool in_pipeline)
{
	if (!cmd || cmd->next != NULL)
		return (ERROR);
	if (cmd->type == AST_CMD)
		return (f_exec_simple_cmd(cmd, env, in_pipeline));
	else if (cmd->type == AST_SUBSHELL)
		return (f_exec_subshell(cmd, env));
	else
		return (ERROR);
}

void	f_cleanup_pipeline(pid_t *pid_list, int created_processes, int temp_fd)
{
	int	i;

	if (temp_fd != STDIN_FILENO)
		close(temp_fd);
	i = 0;
	while (i < created_processes)
	{
		if (pid_list[i] > 0)
			waitpid(pid_list[i], NULL, 0);
		++i;
	}
	if (pid_list)
		free(pid_list);
}

int	f_exec_logic_expr(t_ast_node *expr, t_env *env)
{
	static int	exit_value = 0; //TODO: Important: If 
	//formater put this 0 down exit value needs to
	// be initialized here static int exit_value = 0
	if (!expr || expr->type != AST_LOGICAL_EXPR)
		return (ERROR);
	env->old_exit_value = exit_value;
	exit_value = f_exec_pipeline(expr->data.logical_expr.pipeline, env);
	env->old_exit_value = exit_value;
	if ((expr->data.logical_expr.type == TYPE_AND && exit_value == SUCCESS)
		|| (expr->data.logical_expr.type == TYPE_OR && exit_value != SUCCESS))
		exit_value = f_exec_logic_expr(expr->next, env);
	return (exit_value);
}

int	f_execute_ast(t_env *env)
{
	if (!env->ast_root || env->ast_root->type != AST_LOGICAL_EXPR)
		return (ERROR);
	return (f_exec_logic_expr(env->ast_root, env));
}
