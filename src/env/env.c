/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andmart2 <andmart2@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 14:19:35 by andmart2          #+#    #+#             */
/*   Updated: 2024/05/07 14:19:39 by andmart2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ft_envfull(char *key, char *value)
{
	char	*env_full;
	size_t	key_len;
	size_t	val_len;
	int		i;
	int		j;

	if (!key)
		return (NULL);
	if (!value)
		return (ft_strdup(key));
	key_len = ft_strlen(key);
	val_len = ft_strlen(value);
	env_full = (char *)malloc(key_len + val_len + 2);
	if (!env_full)
		return (NULL);
	i = -1;
	while (key[++i])
		env_full[i] = key[i];
	env_full[i++] = '=';
	j = -1;
	while (value[++j])
		env_full[i + j] = value[j];
	env_full[i + j] = '\0';
	return (env_full);
}

size_t	env_var(t_env *head, int option)
{
	t_env	*tmp;
	size_t	env_cases;

	tmp = head;
	env_cases = 0;
	if (option == 1)
	{
		while (tmp)
		{
			env_cases++;
			tmp = tmp->next;
		}
		return (env_cases);
	}
	else if (option == 2)
	{
		while (tmp->next)
		{
			if (tmp->val)
				env_cases++;
			tmp = tmp->next;
		}
		return (env_cases);
	}
	return (0);
}

void	add_env_to_list(t_toolkit *sh, t_env *new_env)
{
	t_env	*head;
	t_env	*tmp;

	head = sh->env_lst;
	tmp = sh->env_lst;
	if (new_env == NULL)
		return ;
	if (head == NULL)
	{
		sh->env_lst = new_env;
		return ;
	}
	while (tmp->next != NULL)
		tmp = tmp->next;
	tmp->next = new_env;
}

int	first_env(t_toolkit *sh, char **env)
{
	t_env	*s_env;
	int		i;

	i = 0;
	while (env[i] != NULL)
	{
		s_env = malloc(sizeof(t_env));
		if (!s_env)
			return (1);
		s_env->key = ft_substr(env[i], 0, ft_strchr(env[i], '=') - env[i]);
		s_env->val = ft_strdup(getenv(s_env->key));
		s_env->next = NULL;
		add_env_to_list(sh, s_env);
		if (!s_env->key || !s_env->val)
			return (free_env_lst(sh->env_lst));
		i++;
	}
	return (0);
}

char	**env_converter(t_env *env)
{
	t_env			*tmp;
	char			**grid;
	unsigned int	i;

	if (!env)
		return (NULL);
	grid = (char **)malloc(sizeof(char *) * (env_var(env, 1) + 1));
	if (!grid)
		return (NULL);
	tmp = env;
	i = 0;
	while (tmp)
	{
		if (tmp->val != NULL)
		{
			grid[i] = ft_envfull(tmp->key, tmp->val);
			if (!grid[i])
				return (arr_clean(grid, 0));
			i++;
		}
		tmp = tmp->next;
	}
	grid[i] = NULL;
	return (grid);
}
