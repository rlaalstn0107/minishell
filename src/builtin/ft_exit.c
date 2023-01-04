/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kshim <kshim@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/22 12:16:16 by mikim3            #+#    #+#             */
/*   Updated: 2023/01/03 18:44:31 by kshim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_minishell.h"

//에러코드 구현하기

void	ft_exit(t_tree_cmd *cmd, t_pipe *pipe_value)
{
	g_exit_code = 0;
    // exit는 일단 출력
	ft_putendl_fd("exit", pipe_value->outfile_fd); // 이거 설마  STDERR은 아니겠지?
    // ex) mini $>  exit      인자없는경우
	if (cmd->cmd_argv[1] == NULL)
		exit(g_exit_code);  // 그냥 종료하고 상태코드 반환
	g_exit_code = ft_atoi(cmd->cmd_argv[1]);  // 상태코드 저장
	if (cmd->cmd_argv[2] != NULL)
	{
		ft_putstr_fd(SHELL_NAME, STDERR_FILENO);
		ft_putstr_fd(": exit: too many arguments\n", STDERR_FILENO);
		g_exit_code = 1;
	}
	exit(g_exit_code);
	// exit 경우에 따라 정리 잘 된 것 같음. 조금 더 확인. 1/3 kshim.
}
