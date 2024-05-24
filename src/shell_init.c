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

#include "../../includes/minishell.h"

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