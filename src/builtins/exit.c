/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andmart2 <andmart2@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 17:28:10 by andmart2          #+#    #+#             */
/*   Updated: 2024/04/09 17:28:13 by andmart2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <limits.h>

long long int	ft_atol_sh(char *str)
{
	int			i;
	int			sign;
	long long	number;

	sign = 1;
	i = 0;
	number = 0;
	while (str[i] == '\t' || str[i] == '\n' || str[i] == '\v' || str[i] == '\f'
		|| str[i] == '\r' || str[i] == ' ')
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i++] == '-')
			sign = -1;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		if ((number * sign) > (LLONG_MAX - (str[i] - '0')) / 10)
			return (p_exit_err(str, 1));
		number = (number * 10) + (str[i] - '0');
		i++;
	}
	if (number * sign < LLONG_MIN)
		return (p_exit_err(str, 1));
	return ((number *= sign));
}

int	arg_count(char **grid)
{
	int	i;

	i = 0;
	while (grid[i] != NULL)
		i++;
	return (i);
}

int	p_exit_err(char *str_error, int option)
{
	if (option == 1)
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(str_error, 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		return (2);
	}
	else
	{
		ft_putstr_fd(str_error, 2);
		return (1);
	}
}

int	check_exit(char *s)
{
	int	i;

	i = 0;
	while (s[i] == ' ')
		i++;
	if (!s[i])
		return (p_exit_err(s, 1));
	if ((s[i] == '-' || s[i] == '+') && s[i + 1] && ft_isdigit(s[i + 1]))
		i++;
	while (s[i] && ft_isdigit(s[i]))
		i++;
	while (s[i] && s[i] == ' ')
		i++;
	if (s[i])
		return (p_exit_err(s, 1));
	return (ft_atol_sh(s));
}

int	ft_exit(t_toolkit *sh)
{
	char	*input;
	int		ex;

	if (sh->args == NULL || ft_strcmp(sh->args, "exit") == 0)
		return (goodbye(sh));
	else
	{
		ex = 2;
		if (arg_count(sh->pipe_lst->cmd) > 2
			&& ft_isdigit(sh->pipe_lst->cmd[1][0]))
			return (p_exit_err("minishell: exit: too many arguments\n", 2));
		if (arg_count(sh->pipe_lst->cmd) > 2
			&& !ft_isdigit(sh->pipe_lst->cmd[1][0])
			&& ft_isdigit(sh->pipe_lst->cmd[1][0]))
			ft_putstr_fd(": numeric argument required\n", 2);
		sh->power_on = 0;
		input = sh->pipe_lst->cmd[1];
		printf("ciao stronzo\n");
		if (input[0] == '\0')
			p_exit_err(input, 1);
		else
			ex = check_exit(input);
	}
	return (ex);
}
