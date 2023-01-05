/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_syntax_analysis.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kshim <kshim@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/19 11:41:04 by kshim             #+#    #+#             */
/*   Updated: 2023/01/05 19:06:37 by kshim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_minishell.h"

int	ft_syntax_analysis(t_list *token_list)
{
	if (ft_token_type(token_list) == TKN_NULL)
		return (FT_SUCCESS);
	if (ft_stx_a_pipeline(token_list, token_list, 1) == -1)
	{
		ft_putstr_fd(SHELL_NAME, STDERR_FILENO);
		ft_putstr_fd(": syntax error near unexpected token", STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		g_exit_code = 258;
		ft_free_tokenizer_list_and_token(&token_list, 0, TKN_TKNIZE_SUCCESSED);
		return (FT_ERROR);
	}
	return (FT_SUCCESS);
}

int	ft_stx_a_pipeline(t_list *token_list, t_list *token, int token_pos)
{
	token = ft_lst_num(token_list, token_pos);
	if (token == 0)
		return (-1);
	if (ft_token_type(token) == TKN_PIPE)
		return (-1);
	token_pos = ft_stx_a_simple_cmd(token_list, token, token_pos);
	if (token_pos == -1)
		return (-1);
	token = ft_lst_num(token_list, token_pos);
	if (ft_token_type(token) == TKN_NULL)
		return (token_pos);
	if (ft_token_type(token) == TKN_PIPE)
	{
		token_pos = ft_stx_a_pipeline(token_list, token->next, token_pos + 1);
		if (token_pos == -1)
			return (-1);
	}
	else
		return (-1);
	return (token_pos);
}

int	ft_stx_a_simple_cmd(t_list *token_list, t_list *token, int token_pos)
{
	int	stack_pos;

	stack_pos = token_pos;
	token_pos = ft_stx_a_cmd_prefix(token_list, token, stack_pos);
	if (token_pos != -1)
	{
		stack_pos = token_pos;
		token = ft_lst_num(token_list, stack_pos);
		if (token == 0)
			return (-1);
		token_pos = ft_stx_a_word_and_cmd_suffix_case(\
			token_list, token, stack_pos);
		if (token_pos == -1)
			return (stack_pos);
	}
	else
	{
		token_pos = ft_stx_a_word_and_cmd_suffix_case(\
			token_list, token, stack_pos);
		if (token_pos == -1)
			return (stack_pos);
	}
	stack_pos = token_pos;
	return (stack_pos);
}

int	ft_stx_a_word_and_cmd_suffix_case(\
	t_list *token_list, t_list *token, int token_pos)
{
	int	stack_pos;

	stack_pos = token_pos;
	token_pos = ft_stx_a_word(token_list, token, stack_pos);
	if (token_pos == -1)
		return (stack_pos);
	else
	{
		stack_pos = token_pos;
		token = ft_lst_num(token_list, stack_pos);
		if (token == 0)
			return (-1);
		token_pos = ft_stx_a_cmd_suffix(token_list, token, stack_pos);
		if (token_pos == -1)
			return (stack_pos);
		else
			return (token_pos);
	}
	return (stack_pos);
}
