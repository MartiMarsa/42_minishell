/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmarsa-s <mmarsa-s@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 17:03:45 by mmarsa-s          #+#    #+#             */
/*   Updated: 2024/03/19 17:03:47 by mmarsa-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
	This function adds the newly created file descriptor node to the end
	of the list. It ensures that the list is properly linked, maintaining
	the sequence of redirections as they appear in the command.
*/
void	fd_add(t_fd **hd_list, t_fd *new)
{
	t_fd	*tmp;

	if (!*hd_list)
	{
		*hd_list = new;
		return ;
	}
	tmp = *hd_list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

/*
	This function initializes the file descriptor node with the correct
	values based on the lexer token.
	It handles specific redirection types (>, <, >>, <<) by setting up the
	appropriate file descriptor values and ensuring that the file paths
	or contents are correctly expanded and assigned.
*/
int	fd_init(t_fd *new, t_toolkit *tool, int fd)
{
	new->exp = 0;
	new->token = tool->lex_lst->token;
	tool->lex_lst = tool->lex_lst->next;
	while (tool->lex_lst && tool->lex_lst->token == SPACE)
		tool->lex_lst = tool->lex_lst->next;
	if (!tool->lex_lst)
		return (1);
	tool->lex_lst->str = exp_fd(tool, tool->lex_lst->str, new);
	if (!tool->lex_lst->str)
		return (1);
	new->str = tool->lex_lst->str;
	new->fd = fd;
	new->next = NULL;
	if (tool->lex_lst)
		tool->lex_lst = tool->lex_lst->next;
	return (0);
}