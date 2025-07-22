/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eberkau <eberkau@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 00:29:30 by eberkau           #+#    #+#             */
/*   Updated: 2025/07/22 09:17:52 by eberkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "../libft/libft.h"
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <fcntl.h>

# define FALSE 0
# define TRUE 1

typedef struct s_functiondict
{
	char	*call;
	void	(*func)(void);
}	t_funcdict;

enum e_callfunction
{
	INPUT_INVALID = -1,
	CALL_ECHO,
	CALL_CD,
	CALL_PWD,
	CALL_EXPORT,
	CALL_UNSET,
	CALL_ENV,
	CALL_EXIT,
	INPUT_COUNT
};

#endif