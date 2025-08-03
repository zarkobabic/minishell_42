/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   identifier.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbabic <zbabic@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 21:04:03 by zbabic            #+#    #+#             */
/*   Updated: 2025/08/04 00:22:36 by zbabic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdlib.h>
#include "../libft/libft.h"

bool	check_env_var_naming(char *str)
{
	if(!str)
		return false;
	if(str[0] == '?')
		return true;
	if(!(ft_isalpha(str[0]) || str[0] == '_'))
		return (false);
	return true;
}

long	get_env_var_name_len(char *str)
{
	long i;

	if(!str || !check_env_var_naming(str))
		return 0;
	i = 0;
	if(str[0] == '?')
		return 1;
	while(str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		++i;
	return i;
}

long	get_status_len()
{
	//TODO: Find the way to return EXIT status of the last executed command
	return 0;
}

long	count_env_var(char *str, long var_name_len)
{
	char	*var_name;
	char	*sys_env_string;
	
	if(str[0] == '?' && var_name_len == 1)
		return get_status_len();
	var_name = (char*)malloc(var_name_len + 1);
	if(!var_name)
		return -1;
	var_name = ft_memcpy(var_name, str, var_name_len);
	var_name[var_name_len] = '\0';
	sys_env_string = getenv(var_name);
	free(var_name);
	return (long)(ft_strlen(sys_env_string));
}

long	identifier_count_new_string_len(char *str)
{
	long	char_cnt;
	long	i;
	bool	in_single_quotes;
	long	var_name_len;
	long	var_cnt;

	in_single_quotes = false;
	i = 0;
	char_cnt = 0;
	while(str[i])
	{
		if(str[i] == '$' && !in_single_quotes)
		{
			var_name_len = get_env_var_name_len(&str[i + 1]);
			if(var_name_len > 0) //if var_name_len returns something that is 0 or less that means that the name of the variable is either wrong or non existing (NULL) in those cases bash will interpret $ as a normal synmol, we will just do ++i at the end of the loop just like with the normal symbols
			{
				++i;
				var_cnt = count_env_var(&str[i], var_name_len);
				if(var_cnt < 0) //zero len variable expansion is valid (we expand nothing)
					return var_cnt; //allocation failed
				char_cnt += var_cnt;
				i += (var_name_len - 1);
			}
		}
		else
		{
			++char_cnt;
			if(str[i] == '\'')
				in_single_quotes = !in_single_quotes;
		}
		++i;
	}
	return char_cnt;
}

// /// @brief Returns NULL if the variable naming is bad or allocation failed
// /// @param str 
// /// @return 
// char	**identifier_process_to_char_list(char *str)
// {
// 	if(!str)
// 		return NULL; //invalid case, argument cannot be null, it is at least empty string
// 	if(identifier_count_new_string_len(str) < 0)
// 		return NULL; //allocation failed
// 	// identifier_expand_environment_variables(str);
// }


#include <stdio.h>
int	main(void)
{
	char *str = "mama$USER tata'$USER'";
	long	len;

	if(!str)
		len = -67; //invalid case, argument cannot be null, it is at least empty string
	else
		len = identifier_count_new_string_len(str);
	printf("%ld", len);
	return 0;
}