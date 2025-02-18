/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andmart2 <andmart2@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 17:29:12 by andmart2          #+#    #+#             */
/*   Updated: 2024/04/09 17:29:14 by andmart2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_pwd(t_toolkit *sh, t_pipe *p)
{
	char	*pwd;
	int		output;

	output = p->out_fd;
	if (output < 0)
		output = 1;
	pwd = NULL;
	pwd = getcwd(pwd, 0);
	if (!pwd)
	{
		ft_putstr_fd(ft_get_value(sh, "PWD"), output);
	}
	else
	{
		ft_putstr_fd(pwd, output);
		free(pwd);
	}
	ft_putstr_fd("\n", output);
	return (0);
}

int	update_oldpwd(t_toolkit *sh)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, PATH_MAX) == NULL)
		return (-1);
	if (env_val_update(sh->env_lst, "OLDPWD", cwd))
		return (1);
	return (0);
}

int	goodbye(t_toolkit *sh)
{
	sh->power_on = 0;
	printf("ciao stronzo\n");
	return (sh->exit);
}
