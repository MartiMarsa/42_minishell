/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmarsa-s <mmarsa-s@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 17:03:45 by mmarsa-s          #+#    #+#             */
/*   Updated: 2024/03/19 17:03:47 by mmarsa-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
	This function adds the newly created file descriptor node to the end
	of the list. It ensures that the list is properly linked, maintaining
	the sequence of redirections as they appear in the command.
*/
void	fd_add(t_fd **hd_list, t_fd *new)
{
	t_fd	*tmp;

	if (!*hd_list)
	{
		*hd_list = new;
		return ;
	}
	tmp = *hd_list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

/*
	This function initializes the file descriptor node with the correct
	values based on the lexer token.
	It handles specific redirection tokens (>, <, >>, <<) by setting up the
	appropriate file descriptor values and ensuring that the file paths
	or contents are correctly expanded and assigned.
*/
int	fd_init(t_fd *new, t_toolkit *tool, int fd)
{
	new->exp = 0;
	new->token = tool->lex_lst->token;
	tool->lex_lst = tool->lex_lst->next;
	while (tool->lex_lst && tool->lex_lst->token == MYSPACE)
		tool->lex_lst = tool->lex_lst->next;
	if (!tool->lex_lst)
		return (1);
	tool->lex_lst->str = exp_fd(tool, tool->lex_lst->str, new);
	if (!tool->lex_lst->str)
		return (1);
	new->str = tool->lex_lst->str;
	new->fd = fd;
	new->next = NULL;
	if (tool->lex_lst)
		tool->lex_lst = tool->lex_lst->next;
	return (0);
}

/*
	This function is designed to clean up a linked list of file descriptors
	and close any open file descriptors if they are bigger than 0.
*/
void	fd_clean(t_fd **hd, int flag)
{
	t_fd	*temp;
	t_fd	*iter;

	iter = *hd;
	while (iter)
	{
		temp = iter;
		iter = iter->next;
		if (temp->str && flag)
			temp->str = ft_memdel(temp->str);
		if (temp->fd > 0 && flag)
			close(temp->fd);
		temp = ft_memdel(temp);
	}
	*hd = NULL;
}

int	ft_open_built(t_toolkit *sh, t_pipe *p, t_fd *fd1, int prev)
{
	while (fd1)
	{
		ft_check_open(p, fd1, prev);
		if (fd1->exp == 1)
			return (err_break(sh, fd1->str, "ambiguous redirect", 1));
		if (fd1->token == HEREDOC || fd1->token == HEREDOC_NO_EXP)
			p->in_fd = fd1->fd;
		else if (!fd1->str || *fd1->str == '\0')
			return (err_break(sh, "", "No such file or directory", 1));
		else if (fd1->token == 4)
			p->in_fd = open(fd1->str, O_RDONLY);
		else if (fd1->token == 5)
			p->out_fd = open(fd1->str, O_TRUNC | O_CREAT | O_RDWR, 0666);
		else if (fd1->token == 7)
			p->out_fd = open(fd1->str, O_APPEND | O_CREAT | O_RDWR, 0666);
		if (p->in_fd < 0 && (fd1->token == HEREDOC
				|| fd1->token == HEREDOC_NO_EXP || fd1->token == 4))
			return (err_break(sh, fd1->str, NULL, 1));
		if (p->out_fd < 0 && (fd1->token == 5 || fd1->token == 7))
			return (err_break(sh, fd1->str, NULL, 1));
		prev = fd1->token;
		fd1 = fd1->next;
	}
	return (0);
}

char	*ft_smart_join(char *s1, char *s2, char *s3)
{
	char	*new;
	int		i;
	int		j;

	i = -1;
	j = 0;
	if (!(s1 && s2 && s3))
		return (0);
	new = malloc(ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3) + 1);
	if (!new)
		return (NULL);
	while (s1[++i])
		new[i] = s1[i];
	while (s2[j])
		new[i++] = s2[j++];
	j = 0;
	while (s3[j])
		new[i++] = s3[j++];
	new[i] = '\0';
	return (new);
}
