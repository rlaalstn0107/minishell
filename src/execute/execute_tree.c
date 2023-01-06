/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_tree.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kshim <kshim@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/25 23:21:44 by mikim3            #+#    #+#             */
/*   Updated: 2023/01/06 12:41:13 by kshim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_minishell.h"

int	next_pipe_check(t_tree_node *node, t_pipe *m_pipe)
{
	if (node->right != NULL && node->right->type == NODE_PIPELINE)
	{
		if (ft_pipe(m_pipe->pipe) < 0)
			return (FT_ERROR);
		m_pipe->next_pipe_check = BOOL_TRUE;
	}
	else
		m_pipe->next_pipe_check = BOOL_FALSE;
	return (FT_SUCCESS);
}

// kshim execute_redir나 execute_cmd가 실패할 경우 추가 처리가 따로 필요한가?
	// 1.6 kshim -> 여기서 에러 상태는 g_exit_code로 설정하고, 이걸 traversal 함수에서 감지하고 있다.
	// 그래서 traversal 함수의 결과가 error일 때 redir, cmd를 모두 포괄하는 함수를 한 번만 수행하거나,
	// redir와 cmd의 에러에 따른 동작을 분화할 필요가 있다.
void	ft_execute_tree(t_tree_node *node, \
	t_detower *dll_envp_tower, t_pipe *m_pipe)
{
	if (node->type == NODE_REDIRECTIONS)
	{
		if (node->left == 0)
			return ;
	}
	else if (node->type == NODE_REDIR || node->type == NODE_FD_REDIR)
	{
		execute_redir(node, m_pipe);
	}
	else if (node->type == NODE_CMD)
	{
		execute_cmd(node, dll_envp_tower, m_pipe);
	}
	return ;
}

int	ft_mnsh_tree_pre_traversal(t_tree_node *token_tree, \
	t_detower *dll_envp_tower, t_pipe *m_pipe, \
	void (*function)(t_tree_node *, t_detower *, t_pipe *))
{
	if (g_exit_code != 0)
		return (FT_ERROR);
	if ((*function) == BOOL_FALSE)
		return (FT_ERROR);
	function((t_tree_node *)token_tree, \
		(t_detower *)dll_envp_tower, (t_pipe *)m_pipe);
	if (token_tree->left != 0)
	{
		if (ft_mnsh_tree_pre_traversal(token_tree->left, \
			dll_envp_tower, m_pipe, (*function)) == FT_ERROR)
			return (FT_ERROR);
	}
	if (token_tree->right != 0)
	{
		if (ft_mnsh_tree_pre_traversal(token_tree->right, \
			dll_envp_tower, m_pipe, (*function)) == FT_ERROR)
			return (FT_ERROR);
	}
	return (FT_SUCCESS);
}
