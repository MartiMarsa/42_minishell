/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanser_structure.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmarsa-s <mmarsa-s@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 16:17:07 by mmarsa-s          #+#    #+#             */
/*   Updated: 2024/05/14 16:17:08 by mmarsa-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
	Cleans up specific fields within the expansion structure.
*/
void	exp_spc_clean(t_exp *exp)
{
	if (exp->val && exp->alloc)
		exp->val = ft_memdel(exp->val);
	if (exp->var)
		exp->var = ft_memdel(exp->var);
	exp->alloc = 0;
	exp->k = -1;
}

/*
	Fully cleans up the expansion structure, including nested cleanups.
*/
void	exp_clean(t_exp **exp)
{
	if (!(*exp))
		return ;
	exp_nano_clean(*exp);
	(*exp)->str = NULL;
	(*exp)->new = NULL;
	*exp = ft_memdel(*exp);
}

/*
    Initializes the expansion environment, preparing necessary structures
    and variables.
*/
int	exp_init(t_toolkit *tool)
{
	if (tool->exp)
		exp_clean(&tool->exp);
	tool->exp = malloc(sizeof(t_exp));
	if (!tool->exp)
		return (1);
	tool->exp->str = NULL;
	tool->exp->var = NULL;
	tool->exp->val = NULL;
	tool->exp->alloc = NULL;
	tool->exp->k = -1;
	tool->exp->j = -1;
	return (0);
}

/*
	Prepares for the expansion process by setting up necessary
	variables and allocating memory for the expanded string.
*/
int	exp_start(t_toolkit *tool, char *str, int token)
{
	tool->exp->str = str;
	tool->exp->k = new_len(tool, str, token);
	if (tool->exp->k < 0)
		return (1);
	tool->exp->new = malloc(tool->exp->k + 1);
	if (!tool->exp->new)
		return (1);
	tool->exp->alloc = 0;
	tool->exp->k = -1;
	tool->exp->j = -1;
	tool->exp->fl = 0;
	tool->exp->q = ' ';
	return (0);
}

/*
	Expands variables within the content string associated
	with a file descriptor.
	Handles specific cases where expansion is necessary and
	manage quotes appropriately.
*/
char	*exp_fd(t_toolkit *tool, char *str, t_fd *new)
{
	char	*s;

	if (tool->lex_lst->token == WORD
		&& check_exp(str, tool->lex_lst->token, -1) < 0)
		return (trim_quotes(str, ' ', ft_strlen(str), -1));
	else if (new->token == HEREDOC
		|| check_exp(str, tool->lex_lst->token, -1) < 0)
		return (str);
	if (tool->exp)
		exp_clean(&tool->exp);
	if (exp_init(tool))
		return (NULL);
	s = expand_str(tool, str, tool->lex_lst->token, -1);
	if (!s)
		return (ft_memdel(str));
	if (check_file_exp(s))
	{
		s = ft_memdel(s);
		new->exp = 1;
		return (str);
	}
	if (tool->lex_lst->token == WORD)
		return (trim_quotes(s, ' ', ft_strlen(s), -1));
	str = ft_memdel(str);
	return (s);
}
