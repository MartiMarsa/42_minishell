/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmarsa-s <mmarsa-s@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 13:05:52 by mmarsa-s          #+#    #+#             */
/*   Updated: 2024/03/19 13:05:53 by mmarsa-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	error_quotes(t_toolkit *tool)
{
	ft_putstr_fd("minishell: syntax error: Quotes not closed\n", 2);
	tool->exit = 258;
	return (1);
}

int	err_char(t_toolkit *sh, int token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token ", 2);
	if (token == 4)
		ft_putendl_fd("<\n", 2);
	else if (token == 5)
		ft_putendl_fd(">\n", 2);
	else if (token == 6)
		ft_putendl_fd("<<\n", 2);
	else if (token == 7)
		ft_putendl_fd(">>\n", 2);
	else if (token == 8)
		ft_putendl_fd("|\n", 2);
	else
		ft_putendl_fd("newline\n", 2);
	sh->exit = 258;
	return (1);
}

/*
	Handles error reporting and cleanup of the program before
	terminating the process. This function should only be called within
	child processes.
*/
int	err_exit(t_toolkit *sh, char *name, char *message, int err)
{
	if (name || message)
		ft_putstr_fd("minishell: ", 2);
	if ((err == 1 && *name && !message) || err == 14 || err == 12)
		perror(name);
	else if (name)
	{
		ft_putstr_fd(name, 2);
		ft_putstr_fd(": ", 2);
	}
	if (message)
	{
		ft_putstr_fd(message, 2);
		ft_putstr_fd("\n", 2);
	}
	sh_clean(sh);
	exit(err);
}

/*
	Handles errors in the parent process. Unlike err_exit, this function
	does not terminate the process. Instead, it sets an error status and
	returns the error code.
 */
int	err_break(t_toolkit *sh, char *name, char *message, int err)
{
	if (name || message)
		ft_putstr_fd("minishell: ", 2);
	if ((err == 1 && *name && !message) || err == 14 || err == 12)
		perror(name);
	else if (name)
	{
		ft_putstr_fd(name, 2);
		ft_putstr_fd(": ", 2);
	}
	if (message)
	{
		ft_putstr_fd(message, 2);
		ft_putstr_fd("\n", 2);
	}
	sh->exit = err;
	return (err);
}

/*error cd*/
void	print_error(char **args)
{
	ft_putstr_fd("minishell: cd: ", 2);
	if (args[2])
		ft_putstr_fd("too many arguments\n", 2);
	else
	{
		perror(args[1]);
	}
}
