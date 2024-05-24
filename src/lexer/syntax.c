/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmarsa-s <mmarsa-s@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 12:35:28 by mmarsa-s          #+#    #+#             */
/*   Updated: 2024/03/19 12:35:30 by mmarsa-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_quotes(char *str)
{
	int	simple;
	int	dble;

	simple = 0;
	dble = 0;
	while(*str)
	{
		if(*str == 34)
			dble++;
		if(*str == 39)
			simple++;
		str++;
	}
	if (simple % 2 != 0 || dble % 2 != 0)
		return (1);
	return (0); 
}
