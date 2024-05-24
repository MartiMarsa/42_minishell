/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_lex_lst.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmarsa-s <mmarsa-s@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 15:18:42 by mmarsa-s          #+#    #+#             */
/*   Updated: 2024/04/02 15:18:44 by mmarsa-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* function to create lexer nodes */

t_lexer	*lex_new(char *info, t_tokens type)
{
	t_lexer	*new_node;

	new_node = malloc(sizeof(t_lexer));
	if (!new_node)
		return (ft_memdel(info));
	new_node->str = info;
	new_node->token = type;
	new_node->next = NULL;
	new_node->prev = NULL;
	return (new_node);
}

/* function to find the last node of the lexer list*/

t_lexer	*lex_last(t_lexer *lex_list)
{
	while (lex_list && lex_list->next)
		lex_list = lex_list->next;
	return (lex_list);
}

/* function to add lexer nodes to the list */

void	lex_add(t_lexer **lst, t_lexer *new)
{
	t_lexer	*last;

	if (!*lst)
	{
		*lst = new;
		new->prev = NULL;
		return ;
	}
	last = lex_last(*lst);
	last->next = new;
	new->prev = last;
}

/* function to clean the list */

int	lst_clear(t_lexer **lst)
{
	t_lexer	*tmp;

	if (*lst == NULL)
		return (0);
	if ((*lst)->str)
		free((*lst)->str);
	tmp = (*lst)->next;
	free(*lst);
	*lst = NULL;
	return (lst_clear(&tmp));
}

/*
	The lex_insert function inserts the newly created lexer nodes into the
	existing lexer list, replacing the old node and maintaining the integrity
	of the linked list. It ensures that all pointers are correctly updated and
	that the old node's memory is freed.
*/
void	lex_insert(t_toolkit *tool, t_lexer *new, t_lexer **lex, t_lexer *temp)
{
	if (!new)
	{
		tool->lex_lst = temp->next;
		if (temp->prev)
			temp->prev->next = temp->next;
		else
			*lex = temp->next;
		if (temp->next)
			temp->next->prev = temp->prev;
		if (temp->str)
			temp->str = ft_memdel(temp->str);
		temp = ft_memdel(temp);
		return ;
	}
	new->prev = temp->prev;
	tool->lex_lst = lex_last(new);
	lex_last(new)->next = temp->next;
	if (temp->prev)
		temp->prev->next = new;
	else
		*lex = new;
	if (temp->str)
		temp->str = ft_memdel(temp->str);
	temp = ft_memdel(temp);
}
