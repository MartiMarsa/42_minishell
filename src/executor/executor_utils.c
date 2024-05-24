/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andmart2 <andmart2@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 16:30:06 by andmart2          #+#    #+#             */
/*   Updated: 2024/04/16 15:25:50 by andmart2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//recomendacion de poner to lower y usar solo strcmp r CmD (con minusculas y mayusculas mezcladas)
int	check_builtin(char **cmd)
{
	if(!cmd || **cmd)
		return(0);
	if(!ft_strncmp(cmd[0], "echo", ft_longer(cmd[0], "echo")))
		return(1);
	if(!ft_strncmp(cmd[0], "cd", ft_longer(cmd[0], "cd")))
		return(2);
	if(!ft_strncmp(cmd[0], "pwd", ft_longer(cmd[0], "pwd")))
		return(3);
	if(!ft_strncmp(cmd[0], "export", ft_longer(cmd[0], "export")))
		return(4);
	if(!ft_strncmp(cmd[0], "unset", ft_longer(cmd[0], "unset")))
		return(5);
	if(!ft_strncmp(cmd[0], "env", ft_longer(cmd[0], "env")))
		return(6);
	if(!ft_strncmp(cmd[0], "exit", ft_longer(cmd[0], "exit")))
		return(7);
	return(0);
}


void	ft_open(t_toolkit *sh, t_pipe *p, t_fd  *fd1, int prev)
{
	//mientras fd1 no sea null
	while(fd1)
	{
		ft_check_open(p, fd1, prev);
		// si descriptor de archivo tiene bandera abierta es una reedireccion ambigua
		if (fd1->exp == 1)
			err_exit(sh, fd1->str, "ambiguous redirect", 1);
		//si es standar y entrada de error, se establece el descriptor del archivo de entrada en el descriptr del archivo
		if (fd1->token == 6 || fd1-> token == 9)
			p->in_fd = fd1->fd;
		// si no tiene un nombre de archivo o esta vacio
		else if(!fd1->str || *fd1->str == '\0')
			err_exit(sh, "","No such file or diectory", 1);
		else if(fd1->token == 4)
			p->in_fd = open(fd1->str, O_RDONLY);
		else if(fd1->token == 5)
			p->out_fd = open(fd1->str, O_TRUNC | O_CREAT | O_RDWR, 0666);
		else if(fd1->token == 7)
			p->out_fd = open(fd1->str, O_APPEND | O_CREAT | O_RDWR, 0666);
		if(p->in_fd < 0 && (fd1->token == 6 || fd1->token == 9  || fd1->token == 4))
			err_exit(sh, fd1->str, NULL, 1);
		if(p->out_fd < 0 && (fd1->token == 5 ||  fd1->token == 7))
			err_exit(sh, fd1->str, NULL, 1);
		prev = fd1->token;
		fd1 = fd1->next;
	}
}

void	ft_check_open(t_pipe *p, t_fd *cur, int prev)
{
	//verifica si el decriptor de entrada es mayor igual que 0 si el tipo es uno de eso y que el anterior no es 6
	if (p->in_fd >= 0 && (cur->token == 6 || cur->token == 9 || cur->token == 4) \
			&& prev !=6) 
	{
		close(p->in_fd);
		p->in_fd = -2;
	}
	//verifica si el desccriptor de salida es mayor igual que 0 
	if (p->out_fd >= 0 && (cur->token == 5 || cur->token ==7))
	{
		close(p->out_fd);
		p->out_fd = -2; 
	}

}

void check_access(t_toolkit *sh, char **cmd, t_pipe *p)
{
	if(!cmd || !(*cmd))
		err_exit(sh, NULL, NULL, 0);
	else if (!(**cmd))
		err_exit(sh, *cmd,"command not found", 127);
	if (ft_strchr(cmd[0], '/'))
	{
		if(access(cmd[0], F_OK) == 0)
		{
			if(access(cmd[0], X_OK) != 0 )
				err_exit(sh, cmd[0], "permission denied", 126);
			p->path =cmd[0];
		}
		else
			err_exit(sh,cmd[0], "No sucha afile or directory",127); 		
	}
	else
		check_paths(sh->paths, cmd[0], sh, p);
}

void	check_paths(char **paths, char *cmd, t_toolkit *sh, t_pipe *pipe )
{
	int i;

	i = 0;
	if(!paths)
		err_exit(sh, cmd, "No such file or directory", 127);
	while(paths[i])
	{
		//se crea la ruta competa
		pipe->path = ft_smart_join(paths[i],"/",cmd);
		//si no se pudo reservsr la memoria se da error
		if(!pipe->path)
			err_exit(sh, "malloc",NULL, 12);
		//se verifica si el comando existe
		if(access(pipe->path, F_OK) == 0)
		{
		//se verific si tiens permiso de ejecucion
			if (access(pipe->path, X_OK) != 0)
				err_exit(sh, cmd, "permission denied", 126);
			else
				return;	
		}
		//si no existe se libera la memoria
		pipe->path = ft_memdel(pipe->path);
		i++;
	}
	err_exit(sh,cmd, "command not found", 127);	
}



