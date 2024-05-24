/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanser.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmarsa-s <mmarsa-s@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 17:52:42 by mmarsa-s          #+#    #+#             */
/*   Updated: 2024/05/07 17:52:43 by mmarsa-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
	Handles the expansion of variables within heredoc content based on the
	specified type.
	It differentiates between types that require expansion and those that don't.
*/
char	*expand_hd(t_toolkit *tool, char *str, int token)
{
	if (token == HEREDOC_NO_EXP)
		return (str);
	if (check_exp(str, 3, -1) < 0)
		return (str);
	if (exp_init(tool))
		return (ft_memdel(str));
	str = expand_str(tool, str, 3, -1);
	return (str);
}

/*
	Expands variables within the given string based on the type, handling
	double quotes and other special characters.
*/
char	*expand_str(t_toolkit *tool, char *str, int token, int i)
{
	if (exp_start(tool, str, token))
		return (NULL);
	while (str[++i])
	{
		if (open_q(tool->exp, str[i], token) || str[i] != '$' || !str[i + 1])
			tool->exp->new[++tool->exp->j] = str[i];
		else
		{
			tool->exp->var = get_var(&str[i + 1]);
			if (!tool->exp->var)
				return (NULL);
			tool->exp->val = check_value(tool, tool->exp->var);
			if (*tool->exp->var == '?' && !tool->exp->val)
				return (NULL);
			while (tool->exp->val && tool->exp->val[++tool->exp->k])
				tool->exp->new[++tool->exp->j] = tool->exp->val[tool->exp->k];
			i += ft_strlen(tool->exp->var);
			exp_spc_clean(tool->exp);
		}
	}
	tool->exp->new[++tool->exp->j] = '\0';
	if (token == DOUBLEQ)
		tool->exp->str = ft_memdel(tool->exp->str);
	return (tool->exp->new);
}

/*
	Reads and extracts a word from a given input string in,
	handling special characters and quotes, and then returning a new
	lexer token with the extracted word.
*/
t_lexer	*rd_word_exp(char *in, int *i, char q, int j)
{
	char	*str;

	while (in[j] && in[j + 1] && check_chr(in[0]) == 2 && in[j + 1] != in[0])
		j++;
	if (in[j] && in[j + 1] && check_chr(in[0]) == 2)
		j = j + 2;
	while (in[j] && in[j + 1] && in[j + 1] != ' ' && check_chr(in[j + 1]) != 2)
		j++;
	while (in[j] && in[j + 1] && (in[j + 1] == '\'' || in[j + 1] == '\"'))
		j = word_in_quotes(in, &q, j);
	str = ft_substr(in, 0, j + 1);
	if (str)
		str = trim_quotes(str, ' ', ft_strlen(str), -1);
	if (!str)
		return (NULL);
	*i += j;
	return (lex_new(str, 1));
}

/*
	Expands the word in the lexer list by processing the string,
	handling spaces	and word expansions, and then inserting the 
	expanded words back into the lexer list.
*/
int	expand_word(t_toolkit *tool, t_lexer **lex)
{
	char	*str;
	t_lexer	*new;
	t_lexer	*head;
	int		i;

	str = expand_str(tool, tool->lex_lst->str, 1, -1);
	if (!str)
		return (1);
	head = NULL;
	i = -1;
	while (str[++i])
	{
		if (str[i] == ' ')
			new = rd_space(&str[i], &i);
		else
			new = rd_word_exp(&str[i], &i, ' ', 0);
		if (!new)
			return (1);
		lex_add(&head, new);
		if (!str[i])
			break ;
	}
	lex_insert(tool, head, lex, tool->lex_lst);
	str = ft_memdel(str);
	return (0);
}

/*
	Function designed to expand the $ into the command.
*/
int	expanser(t_toolkit *t, t_lexer *head, int flag)
{
	if (exp_init(t))
		return (1);
	while (t->lex_lst)
	{
		if (t->lex_lst->token == 3 && check_exp(t->lex_lst->str, 3, -1) >= 0)
		{
			t->lex_lst->str = expand_str(t, t->lex_lst->str, 3, -1);
			if (!t->lex_lst->str)
				return (err_break(tool_re(&t, head, NULL),
						"malloc", NULL, 12));
		}
		else if (!flag)
		{
			if (exp_quotes(t, &head, &flag))
				return (err_break(tool_re(&t, head, NULL), "malloc", NULL, 12));
		}
		if (t->lex_lst && t->lex_lst->token > 0
			&& t->lex_lst->token < 4 && flag)
			flag = 0;
		if (t->lex_lst)
			t->lex_lst = t->lex_lst->next;
	}
	shell_re(&t, head, NULL);
	return (0);
}
