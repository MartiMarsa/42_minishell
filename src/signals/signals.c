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

#include "../../includes/minishell.h"

void	norm_handler(int sig, siginfo_t *data, void *non_used_data)
{
	(void)data;
	(void)non_used_data;
	if (sig == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 1);
		rl_redisplay();
		g_sig_rec = 1;
	}
	return ;
}

void	do_sigign(int signum)
{
	struct sigaction	signal;

	signal.sa_handler = SIG_IGN;
	signal.sa_flags = SA_RESTART;
	sigemptyset(&signal.sa_mask);
	if (sigaction(signum, &signal, NULL) < 0)
		exit(1);
}

int	init_signals(int mode)
{
	struct sigaction	signal;

	signal.sa_flags = SA_RESTART | SA_SIGINFO;
	sigemptyset(&signal.sa_mask);
	if (mode == NORM)
		signal.sa_sigaction = norm_handler;
	sigaction(SIGINT, &signal, NULL);
	sigaction(SIGQUIT, &signal, NULL);
	return (0);
}

void	exit_status(t_toolkit *sh, int j)
{
	while (++j <= sh->pipes)
	{
		if (sh->exe->pid == wait(&sh->exe->stat))
		{
			if (WIFEXITED(sh->exe->stat))
				sh->exit = WEXITSTATUS(sh->exe->stat);
			else if (WIFSIGNALED(sh->exe->stat))
			{
				if (WTERMSIG(sh->exe->stat) == SIGINT)
				{
					printf("\n");
					sh->exit = 130;
				}
				else if (WTERMSIG(sh->exe->stat) == SIGQUIT)
				{
					sh->exit = 131;
					printf("Quit: 3\n");
				}
			}
		}
	}
}
