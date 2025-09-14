/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_signals.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 05:25:50 by eberkau           #+#    #+#             */
/*   Updated: 2025/09/14 06:20:34 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXE_SIGNALS_H
# define EXE_SIGNALS_H

# include "exe_execution.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>

# define BASE_SIGNO 128

typedef enum		e_sigmode
{
	NONINTERACTIVE = 0,
	HEREDOC,
	INTERACTIVE,
	SILENT
} t_sigmode;

typedef struct s_child_process_data
{
	t_env	*env;
	int		*fd;
	char	*input;
	char	*target;
	char	**argv;
}			t_child;

void		f_set_signals(t_sigmode mode, t_env *env);
void		f_register_birth_control(t_env *env, t_child *data);
t_child		*f_get_struct(void);
void		f_wash_child(t_child *data);

#endif