/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmarsa-s <mmarsa-s@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 15:46:34 by mmarsa-s          #+#    #+#             */
/*   Updated: 2024/03/26 15:46:37 by mmarsa-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
	Check if there is special symbol (<, >, |) and there are quotes,
	for expansion.
	1. return (1) - the char is NOT a redirection, it's a simple char
	2. return (0) - the char is a redirection or space
	3. return (2) - the char is a quote (for expansion)
	4. return (3) - the char is a number (for expansion)
	4. return (4) - the char is a character (for expansion)
*/
int	check_chr(char c)
{
	if (c == 34 || c == 39)
		return (2);
	if ((c > 47 && c < 58) || c == 63)
		return (3);
	else if ((c > 64 && c < 91) || (c > 96 && c < 123) || c == 95)
		return (4);
	else if ((c > 32 && c < 60) || c == '=' || (c > 62 && c < 124) || (c > 124
			&& c < 127))
		return (1);
	return (0);
}

int	word_in_quotes(char *input, char *q, int j)
{
	*q = input[++j];
	j++;
	while (input[j] && input[j] != *q)
		j++;
	while (input[j] && input[j + 1] && input[j + 1] != ' ' && \
		check_chr(input[j + 1]) != 2)
		j++;
	return (j);
}

int	len_no_q(char *s, char q, int len, int i)
{
	int	flag;

	flag = 1;
	while (s[++i])
	{
		if (check_chr(s[i]) == 2 && flag > 0)
		{
			q = s[i];
			flag *= -1;
			len--;
		}
		else if (check_chr(s[i]) == 2 && flag < 0 && s[i] == q)
		{
			flag *= -1;
			len--;
		}
	}
	return (len);
}

/*
	str: The string from which quotes need to be removed.
	q: Initially a space (' ') character, dynamically changes based on
	encountered quotes.
	len: The length of the string s.
	i: Starting index for loops, beginning at -1.
*/
char	*trim_quotes(char *str, char q, int len, int i)
{
	char	*m;
	int		flag;
	int		j;

	if (!str || !len || (!ft_strchr(str, '\'') && !ft_strchr(str, '\"')))
		return (str);
	len = len_no_q(str, q, len, -1);
	m = (char *)malloc(len + 1);
	if (m == 0)
		return (NULL);
	flag = 1;
	j = 0;
	while (++i < len && str[i + j])
	{
		while (check_chr(str[i + j]) == 2 && \
		(flag > 0 || (flag < 0 && str[i + j] == q)))
		{
			flag *= -1;
			q = str[i + j++];
		}
		m[i] = str[i + j];
	}
	m[i] = '\0';
	str = ft_memdel(str);
	return (m);
}

/*
	Cleans up a double array of strings. It either frees each string and
	the array itself or sets each string pointer to NULL without freeing
	them, based on the provided flag.

	flag = 0: Frees all strings in the array and the array itself.
	flag = 1: Sets all string pointers in the array to NULL without
	freeing them, then frees the array itself.
*/
char	**arr_clean(char **cmd, int flag)
{
	int	i;

	i = 0;
	if (!cmd)
		return (NULL);
	while (cmd && cmd[i])
	{
		if (!flag)
			cmd[i] = ft_memdel(cmd[i]);
		else
			cmd[i] = NULL;
		i++;
	}
	cmd = ft_memdel(cmd);
	return (NULL);
}
