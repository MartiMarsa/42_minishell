/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andmart2 <andmart2@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 15:56:38 by andmart2          #+#    #+#             */
/*   Updated: 2024/04/16 16:11:50 by andmart2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
# include "../../include/minishell.h"


int	init_signals(int mode)
{
	struct sigaction signal;

	signal.sa_flags = SA_RESTART | SA_SIGINFO;
	sigemptyset(&signal.sa_mask);
	if(mode == NORM)
		signal.sa_sigaction = norm_handler;
	sigaction(SIGINT, &signal, NULL);
	sigaction(SIGQUIT, &signal, NULL);
	return(0);
}
