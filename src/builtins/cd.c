/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andmart2 <andmart2@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 17:27:26 by andmart2          #+#    #+#             */
/*   Updated: 2024/04/16 12:50:31 by andmart2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <limits.h>
#include <unistd.h>

void print_error(char **args)
{
	ft_putstr_fd("minishell: cd: ", 2);
	if(args[2])
		ft_putstr_fd("too many arguments\n", 2);
	else
	{
		perror(args[1]);//poner este error
	}
}

int	update_oldpwd(t_toolkit *sh)
{
	char cwd[PATH_MAX];

	if (getcwd(cwd, PATH_MAX) == NULL)
		return(-1);
	if (env_val_update(sh->env_lst, "OLDPWD", cwd))
		return(1);
	return(0);
}

int	go_to_path(int option, t_toolkit *sh)
{
	//variable almacenar el valor de retorno
	int		ret;
	//puntero para almacenar camino del entorno
	char	*env_path;
	
	//inicializar
	env_path = NULL;

	if (option == 0)
	{
		//actualizar oldpwd en el entorno
		update_oldpwd(sh);
		env_path = ft_get_value(sh, "HOME");
		if(!env_path)
			ft_putstr_fd("minishell :cd :HOME not set\n", 2);
		if(!env_path)
			return(1);
	}
	else if (option == 1)
	{
		env_path = ft_get_value(sh, "OLDPWD");
		if(!env_path)
			ft_putstr_fd("minishell: cd: OLDPWD not set \n", 2);
		if(!env_path)
			return(1);
		update_oldpwd(sh);
	}
	ret = chdir(env_path);
	return(ret);
}


int ft_cd( t_toolkit *sh, t_pipe *p)
{
	//pointer para guardar comandos
	char	**args;
	//para guardar el retorno el valor
	int		cd_ret;

	//inicializar
	cd_ret = 0;
	//obtener los argumentos del comando de la estructura t_pipe
	args = p->cmd;
 
	//verificar sino se proporciona ningun argumento
	if(!args[1] || args[1][0] == '~')
		return (go_to_path(0, sh));
	//verifica si el argumento es '-'
	if(ft_strcmp(args[1], "-") == 0)
	{
		//cambiar al directorio anterior
		cd_ret = go_to_path(1, sh);
		//actualiza la variable PWD con el valor OLDPWD
		add_or_update_env(sh, "PWD", ft_get_value(sh, "OLDPWD"));
	}
	else
	{
		//actualizar variable de entorno
		update_oldpwd(sh);
		//verificar si el argumento no esta vacio
		if (args[1][0])
			cd_ret = chdir(args[1]);
		//verificar si chdir devolvio error 
		if (cd_ret <0)
			cd_ret *= -1;
		//si chdir no fue exitoso impimir error 
		if (cd_ret != 0)
			print_error(args);
		//actualizar la variable PWD
		add_or_update_env(sh,"PWD", getcwd(NULL, 0));
	}
	//devolver el resultado de operacion de cambio de dirctorio
	return(cd_ret);
}
