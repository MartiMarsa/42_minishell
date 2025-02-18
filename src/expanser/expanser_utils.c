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
if		token = DOUBLEQ - it's a string ""  -  expansion
if		token = SIMPLEQ - it's a string ''  - no expansion
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

/*
	Calculates the new length of a string after performing variable expansions
	based on the provided shell state and type. It handles memory management
	for temporary variables and ensures that the length is updated correctly
	considering the replacements made.
*/
int	new_len(t_toolkit *tool, char *cont, int type, int len)
{
	int		i;
	char	*new;
	char	*val;

	while (*cont)
	{
		i = check_exp(cont, type, -1);
		if (i < 0)
			break ;
		new = get_var(&cont[i + 1]);
		if (!new)
			return (-1);
		val = check_value(tool, new);
		if (!val)
		{
			new = ft_memdel(new);
			return (len);
		}
		len = len - ft_strlen(new) - 1 + ft_strlen(val);
		if (*new == '?')
			val = ft_memdel(val);
		cont = cont + i + 1;
		new = ft_memdel(new);
	}
	return (len);
}

/*
	Extracts a variable name from cont and returns it as a
	dynamically allocated string.
*/
char	*get_var(char *cont)
{
	int		len;
	char	*new;

	len = 0;
	if (!cont)
		return (NULL);
	if (check_chr(cont[len]) == 3)
	{
		new = malloc(2);
		if (!new)
			return (NULL);
		new[0] = cont[0];
		new[1] = '\0';
		return (new);
	}
	while (cont[len] && check_chr(cont[len]) > 2 && cont[len] != 36)
		len++;
	new = malloc(len + 1);
	if (!new)
		return (NULL);
	len = -1;
	while (cont[++len] && check_chr(cont[len]) > 2)
		new[len] = cont[len];
	new[len] = '\0';
	return (new);
}

/*
	Retrieves the value of a given variable name (var)
	from the shell state. It handles special cases for
	certain variable names like ? and 0.
*/
char	*check_value(t_toolkit *tool, char *var)
{
	if (!var)
	{
		return (NULL);
	}
	if (*var == '?')
	{
		tool->exp->alloc = 1;
		return (ft_itoa(tool->exit));
	}
	else if (*var == '0')
		return ("Hola Juan Carlos\0");
	else if (ft_isdigit(*var))
		return ("\0");
	else
	{
		return (ft_get_value(tool, var));
	}
}

/*
Validates a string, ensuring that it does not contain
unquoted spaces and that any quotes within the string are
properly opened and closed.
	flag = 0 - there is no open quotes
	flag = 1 - there is an open quote
*/
int	check_file_exp(char *str)
{
	int		i;
	int		flag;
	char	q;

	i = -1;
	flag = 0;
	q = ' ';
	while (str[++i])
	{
		if (str[i] == ' ' && !flag)
			return (1);
		if (check_chr(str[i]) == 2 && !flag)
		{
			flag++;
			q = str[i];
		}
		else if (flag && str[i] == q)
			flag = 0;
	}
	return (0);
}
