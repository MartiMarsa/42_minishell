/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andmart2 <andmart2@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 17:27:26 by andmart2          #+#    #+#             */
/*   Updated: 2024/04/16 12:50:31 by andmart2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_the_path(t_toolkit *sh, int opt)
{
	char	*env_path;

	env_path = NULL;
	if (opt == 1)
	{
		env_path = ft_get_value(sh, "HOME");
		if (!env_path)
			ft_putstr_fd("minishell : cd: HOME not set\n", 2);
		return (env_path);
	}
	if (opt == 2)
	{
		env_path = ft_get_value(sh, "OLDPWD");
		if (!env_path)
			ft_putstr_fd("minishell : cd: OLDPWD not set\n", 2);
		return (env_path);
	}
	return (env_path);
}

char	*get_cd_path(int option, t_toolkit *sh)
{
	if (update_oldpwd(sh) != 0)
		return (NULL);
	if (option == 0)
		return (get_the_path(sh, 1));
	else if (option == 1)
		return (get_the_path(sh, 2));
	return (NULL);
}

int	go_to_path(int option, t_toolkit *sh)
{
	char	*env_path;
	int		ret;
	char	*new_pwd;

	env_path = get_cd_path(option, sh);
	if (!env_path)
		return (1);
	ret = chdir(env_path);
	if (ret == 0 && option == 1)
	{
		new_pwd = getcwd(NULL, 0);
		if (new_pwd)
		{
			printf("%s\n", new_pwd);
			free(new_pwd);
		}
	}
	return (ret);
}

int	change_directory(t_toolkit *sh, const char *path)
{
	int		cd_ret;
	char	*new_pwd;

	cd_ret = 0;
	if (path && path[0] && update_oldpwd(sh) == 0)
	{
		cd_ret = chdir(path);
		if (cd_ret != 0)
		{
			cd_ret *= -1;
			print_error((char *[]){"cd", (char *)path, NULL});
		}
		else
		{
			new_pwd = getcwd(NULL, 0);
			if (new_pwd)
			{
				add_or_update_env(sh, "PWD", new_pwd);
				free(new_pwd);
			}
		}
	}
	return (cd_ret);
}

int	ft_cd(t_toolkit *sh, t_pipe *p)
{
	char	**args;
	char	*oldpwd;

	args = p->cmd;
	if (!args[1] || args[1][0] == '~')
		return (go_to_path(0, sh));
	if (ft_strcmp(args[1], "-") == 0)
	{
		if (go_to_path(1, sh) == 0)
		{
			oldpwd = ft_get_value(sh, "OLDPWD");
			if (oldpwd)
				add_or_update_env(sh, "PWD", oldpwd);
			return (0);
		}
		return (1);
	}
	return (change_directory(sh, args[1]));
}
