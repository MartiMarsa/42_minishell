/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andmart2 <andmart2@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 17:27:44 by andmart2          #+#    #+#             */
/*   Updated: 2024/04/16 13:29:15 by andmart2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_echo(t_toolkit *sh, t_pipe *p)
{
	char **cmd;
	int has_n;
	int output;

	cmd = p->cmd;
	has_n = check_nl(cmd[1]);
	output = p->out_fd;

	if(output < 0)
		output = 1;
	if(num_args(cmd) > 1)
		echo_args(cmd, output);
	if(has_n)
		ft_putstr_fd("\n", output);
	if(sh->pipes)
		err_break(sh, NULL, NULL, 0);
	return(0);
}
