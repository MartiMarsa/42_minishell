/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_sort.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andmart2 <andmart2@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 14:16:11 by andmart2          #+#    #+#             */
/*   Updated: 2024/05/07 14:16:34 by andmart2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print_sort_print(t_env *env)
{
	t_env	*tmp;
	char	*tmp_name;
	char	*tmp_value;

	tmp = env;
	while (tmp != NULL)
	{
		if (tmp->next != NULL)
		{
			if (ft_strcmp(tmp->key, tmp->next->key) > 0)
			{
				tmp_name = tmp->key;
				tmp_value = tmp->val;
				tmp->key = tmp->next->key;
				tmp->val = tmp->next->val;
				tmp->next->key = tmp_name;
				tmp->next->val = tmp_value;
				tmp = env;
			}
		}
		tmp = tmp->next;
	}
}

void	sort_env(t_env *head)
{
	t_env	*head_bk;

	head_bk = head;
	if (!head_bk)
		return ;
	print_sort_print(head);
	head = head_bk;
}
