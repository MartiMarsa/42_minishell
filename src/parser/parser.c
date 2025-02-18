/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmarsa-s <mmarsa-s@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 18:58:48 by mmarsa-s          #+#    #+#             */
/*   Updated: 2024/05/24 18:58:50 by mmarsa-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
	This function traverses a linked list of tokens and returns the next token
	that has a value between 1 and 3 (inclusive), which means that it could be
	a word or a string between simple or double quotes.
*/
t_lexer	*next_word(t_lexer *temp)
{
	int	f;

	f = 0;
	while (temp && temp->token != PIPE)
	{
		if (temp->token > 3 && !f)
			f++;
		else if (temp->token < 4 && temp->token > 0 && f)
			f = 0;
		else if (temp->token < 4 && temp->token > 0 && !f)
			return (temp);
		temp = temp->next;
	}
	return (NULL);
}

/*
	This function counts the number of command tokens (values between 1 and 3)
	in a linked list until a pipe is encountered.
*/
int	count_cmd(t_lexer *temp)
{
	int	i;
	int	flag;

	i = 0;
	flag = 0;
	while (temp && temp->token != PIPE)
	{
		if (temp->token < 4 && temp->token > 0 && !flag)
			i++;
		else if (temp->token < 4 && temp->token > 0 && flag)
			flag = 0;
		else if (temp->token > 3)
			flag++;
		temp = temp->next;
	}
	return (i);
}

/*
	This function parses command tokens from a lexer list and stores them
	in the cmd array of a t_pipe structure.
*/
int	parse_cmd(t_pipe *new, t_lexer *temp, t_toolkit *t, int j)
{
	int	i;

	i = count_cmd(temp);
	new->cmd = (char **)malloc(sizeof(char *) * (i + 1));
	if (!new->cmd)
		return (1);
	while (t->lex_lst && t->lex_lst->token < 4)
	{
		if (t->lex_lst->token != 0)
		{
			new->cmd[j++] = t->lex_lst->str;
		}
		t->lex_lst = t->lex_lst->next;
	}
	temp = t->lex_lst;
	while (j < i)
	{
		temp = next_word(temp);
		new->cmd[j++] = temp->str;
		temp = temp->next;
	}
	new->cmd[j] = NULL;
	return (0);
}

/*
	This function identifies redirection tokens and initiates the creation
	and initialization of file descriptor nodes.
	It ensures that each redirection token results in a corresponding
	file descriptor node being added to the command's list of redirections.
*/
int	parse_redir(t_pipe *new, t_lexer *lex, t_fd *hd, t_toolkit *t)
{
	t_fd	*fd_new;
	int		check;

	fd_new = NULL;
	check = 0;
	fd_new = malloc(sizeof(t_fd));
	if (!fd_new)
		return (1);
	fd_add(&(new->fd_lst), fd_new);
	if ((lex->token > 3 && lex->token < 6) || lex->token == 7)
		check = fd_init(fd_new, t, -2);
	else if (lex->token == HEREDOC)
	{
		check = fd_init(fd_new, t, hd->fd);
		t->hd_lst = hd->next;
	}
	if (check)
		return (1);
	return (0);
}

/*
	This function reads a list of tokens and organize them into a linked list
	of t_pipe structures,
		where each t_pipe structure contains commands and their
	associated file descriptors.
*/
int	parser(t_toolkit *t, t_lexer *lex, t_fd *hd, t_pipe *new)
{
	while (t->lex_lst)
	{
		new = malloc(sizeof(t_pipe));
		if (!new)
			return (err_break(shell_re(&t, lex, hd), "malloc", NULL, 12));
		pipe_init(new);
		pipe_add(t, new);
		while (t->lex_lst && t->lex_lst->token != PIPE)
		{
			if (t->lex_lst->token > 3 && t->lex_lst->token < 8)
				t->check = parse_redir(new, t->lex_lst, t->hd_lst, t);
			else if (t->lex_lst->token > 0 && t->lex_lst->token < 4
				&& !new->cmd)
				t->check = parse_cmd(new, t->lex_lst, t, 0);
			else
				t->lex_lst = t->lex_lst->next;
			if (t->check)
				return (err_break(shell_re(&t, lex, hd), "malloc", NULL, 12));
		}
		if (t->lex_lst)
			t->lex_lst = t->lex_lst->next;
	}
	shell_re(&t, lex, hd);
	return (0);
}
