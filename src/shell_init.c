/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmarsa-s <mmarsa-s@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 20:15:11 by mmarsa-s          #+#    #+#             */
/*   Updated: 2024/05/24 20:15:13 by mmarsa-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	sh_init(t_toolkit *sh, char **env)
{
	sh->env_lst = NULL;
	sh->lex_lst = NULL;
	sh->hd_lst = NULL;
	sh->pipe_lst = NULL;
	sh->exp = NULL;
	sh->args = NULL;
	sh->paths = NULL;
	sh->exit = 0;
	sh->pipes = 0;
	sh->check = 0;
	sh->exe = NULL;
	sh->env = NULL;
	sh->power_on = 0;
	if (env)
		first_env(sh, env);
	sh->env = env_converter(sh->env_lst);
	sh->power_on = 1;
	return (0);
}

void	sh_clean(t_toolkit *sh)
{
	if (sh->lex_lst)
		lex_clean(&(sh->lex_lst));
	if (sh->hd_lst)
		fd_clean(&(sh->hd_lst), 1);
	if (sh->args)
		sh->args = ft_memdel(sh->args);
	if (sh->pipe_lst)
		pipe_clean(&(sh->pipe_lst));
	if (sh->paths)
		sh->paths = arr_clean(sh->paths, 0);
	if (sh->exe)
		sh->exe = ft_memdel(sh->exe);
	if (sh->exp)
		exp_clean(&sh->exp);
	sh->pipes = 0;
	sh->check = 0;
}

/*
	The shell_re function restores the initial positions of the lexer and
	heredoc lists within the t_toolkit structure to their original states after
	they have been iterated over.
*/
t_toolkit	*shell_re(t_toolkit **tool, t_lexer *lex, t_fd *hd)
{
	if (lex)
		(*tool)->lex_lst = lex;
	if (hd)
		(*tool)->hd_lst = hd;
	return (*tool);
}

int	sh_loop_init(t_toolkit *sh)
{
	sh->pipes = 0;
	sh->check = 0;
	if (ft_get_value(sh, "PATH"))
	{
		sh->paths = ft_split(ft_get_value(sh, "PATH"), ':');
		if (!sh->paths)
			return (err_break(sh, "malloc", NULL, 12));
	}
	if (allocate_exe(sh))
		return (err_break(sh, "malloc", NULL, 12));
	return (0);
}

int	allocate_exe(t_toolkit *sh)
{
	t_exec	*new;

	new = malloc(sizeof(t_exec));
	if (!new)
		return (1);
	new->fdp[0] = -2;
	new->fdp[1] = -2;
	sh->exe = new;
	return (0);
}
