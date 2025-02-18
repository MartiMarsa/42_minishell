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

/*
	This function scans through a string to determine if there are any
	unclosed single or double quotes, taking escaped quotes into account.
	The	return value indicates whether there is an unclosed quote and what
	type it is.
	0 means no quote is currently open.
	1 means a double quote (") is currently open.
	2 means a single quote (') is currently open.
*/
int	check_quotes(char *str)
{
	int	i;
	int	open;

	i = 0;
	open = 0;
	while (str[i])
	{
		if (i > 0 && str[i - 1] == '\\')
			;
		else if (open == 0 && str[i] == '\"')
			open = 1;
		else if (open == 0 && str[i] == '\'')
			open = 2;
		else if (open == 1 && str[i] == '\"')
			open = 0;
		else if (open == 2 && str[i] == '\'')
			open = 0;
		i++;
	}
	return (open);
}

/*
	This function is designed to check if a given input string in strains
	only whitespace characters (specifically, spaces) or is completely empty.
*/
int	check_input(char *in)
{
	while (*in && *in == ' ')
		in++;
	if (!*in)
		return (1);
	return (0);
}

/*
	Ensures that the sequence of tokens follows certain rules, and it
	returns an error if any rule is violated. The specific rules for valid
	syntax are based on the token values.
*/
int	check_syntax(t_toolkit *sh, t_lexer *current, int prev_token)
{
	if (current->token == 8)
		return (err_char(sh, current->token));
	while (current != NULL)
	{
		if (current->token != 0)
		{
			if (current->token >= 4 && current->token <= 8)
			{
				if ((prev_token >= 4 && prev_token < 8) || (prev_token == PIPE
						&& current->token == PIPE))
					return (err_char(sh, current->token));
			}
			prev_token = current->token;
		}
		current = current->next;
	}
	if (prev_token >= 4 && prev_token <= 9)
		return (err_char(sh, prev_token));
	return (0);
}

/*
	Serves to handle token expansion and quotation trimming within
	a lexer structure.
*/
int	exp_quotes(t_toolkit *sh, t_lexer **head, int *flag)
{
	size_t	len;

	if (sh->lex_lst->token == WORD && check_exp(sh->lex_lst->str, 1, -1) >= 0)
	{
		if (expand_word(sh, head))
			return (1);
	}
	else if (sh->lex_lst->token == WORD && check_exp(sh->lex_lst->str, 1,
			-1) < 0)
	{
		len = ft_strlen(sh->lex_lst->str);
		sh->lex_lst->str = trim_quotes(sh->lex_lst->str, ' ', len, -1);
		if (!sh->lex_lst->str)
			return (1);
	}
	else if (sh->lex_lst->token > 3 && sh->lex_lst->token < 8)
		*flag = 1;
	return (0);
}
