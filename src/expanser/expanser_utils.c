/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanser_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmarsa-s <mmarsa-s@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 18:04:08 by mmarsa-s          #+#    #+#             */
/*   Updated: 2024/05/07 18:04:12 by mmarsa-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
if token = DOUBLEQ - it's a string ""  -  expansion
if token = SIMPLEQ - it's a string ''  - no expansion
Q is always starting -1 - its a flag to check if we are in the double quotes. 
q < 0 - no open quotes
q > 1 - you are inside open quotes
*/
int	check_exp(char *str, int token, int q)
{
	int	i;

	i = 0;
	if (!str || (token != WORD && token != DOUBLEQ))
		return (-1);
	while (str[i] && str[i + 1])
	{
		if (token == WORD && str[i] == '\"')
			q *= -1;
		if (token == WORD && q < 0 && str[i] == '\'')
		{
			i++;
			while (str[i] && str[i] != '\'')
				i++;
			if (!str[i])
				break ;
		}
		else if (str[i] == '$' && check_chr(str[i + 1]) > SIMPLEQ)
			break ;
		i++;
	}
	if (!str[i] || !str[i + 1])
		return (-1);
	return (i);
}
