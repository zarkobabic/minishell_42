/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_child_process_management.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 09:37:50 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/14 09:38:29 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exe_execution.h"
#include "exe_redirection.h"
#include <sys/wait.h>

void	f_cleanup_child(t_ast_node *root, char **argv)
{
	ast_free_tree(root);
	if (argv)
		free_argv(&argv);
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
