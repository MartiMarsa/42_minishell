/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmarsa-s <mmarsa-s@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 16:44:23 by mmarsa-s          #+#    #+#             */
/*   Updated: 2024/03/12 16:44:33 by mmarsa-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_sig_rec;

int	minishell_loop(t_toolkit *sh)
{
	init_signals(NORM);
	do_sigign(SIGQUIT);
	sh->args = readline("Hola Juan Carlos$> ");
	if (!sh->args)
		return (ft_exit(sh));
	do_sigign(SIGINT);
	add_history(sh->args);
	if (check_input(sh->args))
		return (0);
	if (check_quotes(sh->args))
		return (error_quotes(sh));
	if (heredoc(sh, sh->args, 0))
		return (1);
	if (lexer(sh, sh->args))
		return (1);
	if (expanser(sh, sh->lex_lst, 0))
		return (1);
	if (!sh->lex_lst || check_syntax(sh, sh->lex_lst, -1))
		return (1);
	if (!sh->lex_lst || parser(sh, sh->lex_lst, sh->hd_lst, NULL))
		return (1);
	if (executor(sh, sh->pipe_lst, -1, -1))
		return (1);
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_toolkit	tool;

	(void)envp;
	if (argc != 1)
	{
		printf("minishell: %s: No such file or directory\n", argv[1]);
		return (127);
	}
	g_sig_rec = 0;
	sh_init(&tool, envp);
	while (tool.power_on)
	{
		if (!sh_loop_init(&tool))
			minishell_loop(&tool);
		sh_clean(&tool);
		if (g_sig_rec == 1)
		{
			tool.exit = 1;
			g_sig_rec = 0;
		}
	}
	free_env(&tool);
	return (tool.exit);
}
