/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmarsa-s <mmarsa-s@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 18:58:59 by mmarsa-s          #+#    #+#             */
/*   Updated: 2024/05/24 18:59:01 by mmarsa-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
	This function sets up the initial state of the t_pipe structure, ensuring
	that all members are properly initialized before the structure is used 
	further in the code.
*/
void	pipe_init(t_pipe *pipe)
{
	pipe->cmd = NULL;
	pipe->path = NULL;
	pipe->fd_lst = NULL;
	pipe->in_fd = -2;
	pipe->out_fd = -2;
	pipe->builtin = 0;
	pipe->next = NULL;
}

/*
	This function is responsible for adding a new t_pipe node to the end of a
	linked list of t_pipe nodes. 
*/
void	pipe_add(t_toolkit *tool, t_pipe *new)
{
	t_pipe	*temp;

	if (!tool->pipe_lst)
	{
		tool->pipe_lst = new;
		return ;
	}
	temp = tool->pipe_lst;
	while (temp->next)
		temp = temp->next;
	temp->next = new;
}