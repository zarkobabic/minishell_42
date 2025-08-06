/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   identifier.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 00:19:46 by zbabic            #+#    #+#             */
/*   Updated: 2025/08/06 19:10:18 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IDENTIFIER_H
# define IDENTIFIER_H

# include <stdbool.h>

char	*identifier_expand_variables_in_string(char *str);
char	**split_by_unquoted_space(const char *str);
char	*remove_quotes(char *str);

bool	wildcard_check_if_matches(char *input, char *pattern);
char	**get_all_matched_names(char *pattern);

#endif
