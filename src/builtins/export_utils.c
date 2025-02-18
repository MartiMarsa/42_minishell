/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andmart2 <andmart2@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 17:28:59 by andmart2          #+#    #+#             */
/*   Updated: 2024/04/09 17:29:01 by andmart2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print_env_fd(t_env *tmp, int output)
{
	ft_putstr_fd("=", output);
	ft_putstr_fd("\"", output);
	ft_putstr_fd(tmp->val, output);
	ft_putstr_fd("\"", output);
	ft_putstr_fd("\n", output);
}

int	print_export(t_env *eprint, t_pipe *p)
{
	t_env	*tmp;
	int		output;

	output = p->out_fd;
	if (output < 1)
		output = 1;
	if (!eprint)
		return (1);
	print_sort_print(eprint);
	tmp = eprint;
	while (tmp)
	{
		ft_putstr_fd("declare -x ", output);
		ft_putstr_fd(tmp->key, output);
		if (tmp->val)
			print_env_fd(tmp, output);
		else
			ft_putstr_fd("\n", output);
		tmp = tmp->next;
	}
	tmp = eprint;
	return (0);
}

char	*find_in_env_variables(t_toolkit *sh, char *variable_name)
{
	t_env	*env;
	int		env_name_len;
	int		var_name_len;

	if (variable_name == NULL)
		return (NULL);
	env = sh->env_lst;
	var_name_len = ft_strlen(variable_name);
	while (env != NULL)
	{
		env_name_len = ft_strlen(env->key);
		if (ft_strncmp(env->key, variable_name, env_name_len) == 0
			&& (env_name_len == var_name_len))
			return (env->val);
		env = env->next;
	}
	return (NULL);
}

int	export_option(const char *name)
{
	int	i;

	if (!name || (!ft_isalpha(name[0]) && name[0] != '_'))
		return (0);
	i = 1;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
		{
			if (name[i] == '+' && name[i + 1])
				return (0);
			if (name[i] == ' ' || name[i] == '%' || name[i] == '/')
				return (0);
			if (name[i] != '+' && name[i] != '=')
				return (0);
		}
		i++;
	}
	return (1);
}
