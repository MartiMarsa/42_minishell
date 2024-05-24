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

int	minishell_loop(t_toolkit *tool)
{
	tool->args = readline("Hola Juan Carlos>$ ");
	if (!tool->args || !ft_strncmp(tool->args, "exit", ft_strlen("exit")))
	{
		printf("adios\n");
		exit(EXIT_SUCCESS);
	}
	add_history(tool->args);
	if (check_quotes(tool->args))
		return (error_quotes(tool));
	//if (heredoc(tool, tool->args, 0))
	//	return (1);
	if (lexer(tool, tool->args))
		return (1);
		
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_toolkit	tool;
	
	(void)envp;
	if(argc != 1 || argv[1])
	{
		printf("This programm doesn't accept arguments.\n");
		exit(0);
	}
	//g_sig_rec = 0;			TO - DO
	//sh_init(&sh, envp);		TO - DO
	tool.power_on = 1;
	while (tool.power_on)
	{
		//if (!sh_loop_init(&tool))		TO - DO
			minishell_loop(&tool);
		/*sh_clean(&tool);
		if (g_sig_rec == 1)
		{
			sh.exit = 1;
			g_sig_rec = 0;
		}*/
	}
	//free_env(&tool);					TO - DO
	return (tool.exit);
}
