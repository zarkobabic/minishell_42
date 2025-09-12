/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_signals_child.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 05:25:11 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/12 10:52:35 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exe_signals.h"

t_child	*f_get_struct(void)
{
	static t_child	*data = NULL;

	if (!data)
		data = (t_child *)malloc(sizeof(t_child));
	return (data);
}

void	f_wash_child(t_child *data)
{
	close(data->fd[OUT]);
	if (data->input)
		free(data->input);
	free(data->target);
	free_argv(&data->argv);
	ast_free_tree(data->env->ast_root);
	tcsetattr(STDIN_FILENO, TCSANOW, &data->env->orig_term);
	free(data);
	rl_on_new_line();
	rl_replace_line("", 0);
}

static void	f_kill_child(int signo)
{
	t_child	*data;
	t_env	*env;

	data = f_get_struct();
	env = data->env;
	f_wash_child(data);
	close(STDIN_FILENO);
	mini_exit(BASE_SIGNO + signo, env);
}

void	f_register_birth_control(t_env *env, t_child *data)
{
	struct sigaction	act_config_child;

	ft_bzero(&act_config_child, sizeof(act_config_child));
	act_config_child.sa_handler = f_kill_child;
	if (sigaction(SIGINT, &act_config_child, NULL) == -1
		|| sigaction(SIGQUIT, &act_config_child, NULL) == -1)
	{
		f_wash_child(data);
		mini_exit(ERROR, env);
	}
}
