/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andmart2 <andmart2@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 14:20:38 by andmart2          #+#    #+#             */
/*   Updated: 2024/05/07 14:20:41 by andmart2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ft_get_value(t_toolkit *sh, char *key)
{
	t_env	*tmp;

	if (!sh || !key)
		return (NULL);
	tmp = sh->env_lst;
	while (tmp)
	{
		if (ft_strncmp(key, tmp->key, ft_longer(key, tmp->key)) == 0)
			return (tmp->val);
		else
			tmp = tmp->next;
	}
	return (NULL);
}
