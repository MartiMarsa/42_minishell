/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmarsa-s <mmarsa-s@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 13:17:38 by mmarsa-s          #+#    #+#             */
/*   Updated: 2024/03/13 13:17:42 by mmarsa-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* Trims all spaces and sets a space token */

t_lexer	*rd_space(char *input, int *i)
{
	char	*new;
	int		j;

	new = NULL;
	j = 0;
	while (input[j + 1] && input[j + 1] == ' ')
		j++;
	new = malloc(sizeof(char) * 2);
	if (!new)
		return (NULL);
	new[0] = ' ';
	new[1] = '\0';
	*i += j;
	return (lex_new(new, SPACE));
}

/* This function serves to parse input strings, 
extracting individual words, including those within quotes */

t_lexer	*rd_word(char *input, int *i, char q)
{
	char	*new;
	int		j;

	new = NULL;
	j = 0;
	while (input[j] && input[j + 1] && check_chr(input[j]) != 2
		&& check_chr(input[j + 1]) != 2 && check_chr(input[j + 1]))
		j++;
	if (check_chr(input[0] == 2))
		j = word_in_quotes(input, &q, -1);
	while (input[j] && input[j + 1] && check_chr(input[j + 1]) == 2)
		j = word_in_quotes(input, &q, j);
	new = ft_substr(input, 0, j + 1);
	if (!new)
		return (NULL);
	*i += j;
	return (lex_new(new, WORD));
}

/* This function identifies the type of special symbol and generates
a LEX_NODE with the defining characteristic */

t_lexer	*rd_symbol(t_toolkit *tool, char *input, int *i)
{
	int			j;

	j = 0;
	if (input[j] == '<' && input[j + 1] != '<')
		return (lex_new(NULL, INFILE));
	else if (input[j] == '>' && input[j + 1] != '>')
		return (lex_new(NULL, OUTFILE));
	else if (input[j] == '>' && input[j + 1] == '>')
	{
		(*i)++;
		return (lex_new(NULL, OUTFILEAPP));
	}
	else if (input[j] == '<' && input[j + 1] == '<')
	{
		(*i)++;
		return (lex_new(NULL, HEREDOC));
	}
	else if (input[j] == '|')
	{
		tool->pipes++;
		return (lex_new(NULL, PIPE));
	}
	return (NULL);
}

/* This function parses an input string, identifying words within 
single or double quotes. It extracts the word within the quotes and 
creates a corresponding lexical token, distinguishing between single
and double quotes. */

t_lexer	*rd_in_quotes(char *input, int *i)
{
	char	*new;
	int		j;

	new = NULL;
	j = 0;
	while (input[j] && input[j + 1] && input[j + 1] != input[0])
		j++;
	if (input[j + 2] && check_chr(input[j + 2]))
		return (rd_word(input, i, ' '));
	new = ft_substr(input, 1, j);
	*i += j;
	if (input[0] == 39)
		return (lex_new(new, SIMPLEQ));
	else if (input[0] == 34)
		return (lex_new(new, DOUBLEQ));
	return (NULL);
}

int lexer(t_toolkit *tool, char	*input)
{
	t_lexer	*new;
	int		i;

	new = NULL;
	i = -1;

	while (input[++i])
	{
		if (input[i] == ' ')
			new = rd_space(&input[i], &i);
		else if (input[i] == '<' || input[i] == '>' || input[i] == '|')
			new = rd_symbol(tool, &input[i], &i);
		else if (input[i] == 39 || input[i] == 34)
			new = rd_in_quotes(&input[i], &i);
		else
			new = rd_word(&input[i], &i, ' ');
		if (!new)
			return (EXIT_FAILURE); // TO - DO error function
		else
			lex_add(&(tool->lex_lst), new);
	}
	return (0);
}