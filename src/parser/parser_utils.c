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

/*
	Deallocates all the resources associated with
	each t_pipe structure in the list, including file
	descriptors, command arrays, and memory buffers, then frees
	the structures.
*/
int	pipe_clean(t_pipe **lst)
{
	t_pipe	*temp;

	while (*lst)
	{
		temp = (*lst)->next;
		if ((*lst)->fd_lst)
			fd_clean(&((*lst)->fd_lst), 0);
		if ((*lst)->in_fd > 0)
			close((*lst)->in_fd);
		if ((*lst)->out_fd > 0)
			close((*lst)->out_fd);
		if ((*lst)->cmd)
			(*lst)->cmd = arr_clean((*lst)->cmd, 1);
		if ((*lst)->path)
			(*lst)->path = ft_memdel((*lst)->path);
		free(*lst);
		*lst = NULL;
		*lst = temp;
	}
	lst = NULL;
	return (1);
}
