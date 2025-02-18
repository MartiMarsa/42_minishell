/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andmart2 <andmart2@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 17:27:44 by andmart2          #+#    #+#             */
/*   Updated: 2024/04/16 13:29:15 by andmart2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	parse_nl(char *s)
{
	int	i;

	i = 1;
	if (!(s[0] == '-'))
		return (0);
	while (s[i] && s[i] == 'n')
		i++;
	if (s[i] != '\0')
		return (0);
	return (1);
}

int	check_nl(char *str)
{
	int	i;

	i = 1;
	if (!str)
		return (1);
	else if (str[0] == '-' && str[1] && str[1] == 'n')
	{
		while (str[i] && str[i] == 'n')
			i++;
		if (!str[i])
			return (0);
	}
	return (1);
}

int	num_args(char **args)
{
	int	i;

	i = 0;
	while (args[i] != NULL)
		i++;
	return (i);
}

void	echo_args(char **cmd, int output)
{
	int	i;

	i = 1;
	while (cmd[i] && parse_nl(cmd[i]))
		i++;
	while (cmd[i])
	{
		ft_putendl_fd(cmd[i], output);
		if (cmd[i + 1])
			ft_putstr_fd(" ", output);
		i++;
	}
}

int	ft_echo(t_toolkit *sh, t_pipe *p)
{
	char	**cmd;
	int		has_n;
	int		output;

	cmd = p->cmd;
	has_n = check_nl(cmd[1]);
	output = p->out_fd;
	if (output < 0)
		output = 1;
	if (num_args(cmd) > 1)
		echo_args(cmd, output);
	if (has_n)
		ft_putstr_fd("\n", output);
	if (sh->pipes)
		err_break(sh, NULL, NULL, 0);
	return (0);
}
