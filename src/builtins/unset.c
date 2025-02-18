/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andmart2 <andmart2@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 17:29:43 by andmart2          #+#    #+#             */
/*   Updated: 2024/04/09 17:29:45 by andmart2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	unset_free(t_env *env)
{
	ft_memdel(env->key);
	ft_memdel(env->val);
	ft_memdel(env);
}

void	unset_var(t_toolkit *sh, char *var)
{
	t_env	*node;
	t_env	*tmp;

	node = sh->env_lst;
	tmp = NULL;
	if (ft_strcmp(node->key, var) == 0)
	{
		sh->env_lst = sh->env_lst->next;
		unset_free(node);
		return ;
	}
	while (node != NULL && ft_strcmp(node->key, var) != 0)
	{
		tmp = node;
		node = node->next;
	}
	if (node == NULL)
		return ;
	tmp->next = node->next;
	unset_free(node);
}

int	ft_unset(t_toolkit *sh, t_pipe *p)
{
	char	**args;
	int		i;

	(void)p;
	args = sh->pipe_lst->cmd;
	i = 1;
	while (args[i])
	{
		unset_var(sh, args[i]);
		i++;
	}
	if (sh->env)
		sh->env = arr_clean(sh->env, 0);
	sh->env = env_converter(sh->env_lst);
	if (!sh->env)
		return (err_break(sh, "malloc", NULL, 12));
	return (0);
}
