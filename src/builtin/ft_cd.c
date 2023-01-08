/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kshim <kshim@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/22 12:15:15 by mikim3            #+#    #+#             */
/*   Updated: 2023/01/03 11:49:58 by kshim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_minishell.h"

void	ft_cd(t_tree_cmd *cmd, t_detower *env_tower)
{
	char		*pwd_path;

	pwd_path = ft_getcwd(NULL, 0);
	if (pwd_path == NULL)
		return ;
	if (cmd->cmd_argv[1] == "~")
		if (ft_chdir(ft_getenv("HOME")) == -1)
		{
			free(pwd_path);
			return ;
		}
	if (cmd->cmd_argv[1] != NULL)
	{
		if (ft_chdir(cmd->cmd_argv[1]) == -1)
		{
			free(pwd_path);
			return ;
		}
	}
	change_pwd(env_tower, "PWD");
	change_oldpwd(env_tower, pwd_path);
	free(pwd_path);
}
