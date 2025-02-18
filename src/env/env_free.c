/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andmart2 <andmart2@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 14:18:41 by andmart2          #+#    #+#             */
/*   Updated: 2024/05/07 14:18:48 by andmart2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	free_env_lst(t_env *head)
{
	t_env	*e_tmp;
	t_env	*prev_node;

	e_tmp = head;
	while (e_tmp)
	{
		if (e_tmp->key)
			e_tmp->key = ft_memdel(e_tmp->key);
		if (e_tmp->val)
			e_tmp->val = ft_memdel(e_tmp->val);
		prev_node = e_tmp;
		e_tmp = e_tmp->next;
		ft_memdel(prev_node);
		prev_node = NULL;
	}
	return (1);
}

void	free_env(t_toolkit *sh)
{
	clear_history();
	if (sh->exe)
		free(sh->exe);
	if (sh->env != NULL)
		sh->env = arr_clean(sh->env, 0);
	free_env_lst(sh->env_lst);
	sh->env_lst = NULL;
}
