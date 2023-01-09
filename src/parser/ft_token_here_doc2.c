/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_token_here_doc2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kshim <kshim@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 08:22:03 by kshim             #+#    #+#             */
/*   Updated: 2023/01/09 13:13:41 by kshim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_minishell.h"

int	ft_make_h_doc_wth_expand(\
	char *token_str, t_detower *dll_envp_tower, int is_env_expand)
{
	int		here_doc_fd;
	char	*delimiter;
	int		ret;

	here_doc_fd = -1;
	here_doc_fd = ft_open(
			"/tmp/.mnsh_here_doc.tmp", O_CREAT | O_RDWR | O_TRUNC, 0666);
	if (here_doc_fd == -1)
		return (FT_ERROR);
	delimiter = ft_strjoin(token_str, "\n");
	if (delimiter == 0)
		return (FT_ERROR);
	set_signal(SIG_HERE_DOC, SIG_IGNORE);
	while (1)
	{
		ret = ft_make_h_doc_loop(\
			delimiter, here_doc_fd, dll_envp_tower, is_env_expand);
		if (ret == FT_ERROR || ret == FT_SUCCESS)
			break ;
	}
	ft_close(here_doc_fd);
	return (set_signal(SIG_HANDLER, SIG_IGNORE), free(delimiter), ret);
}

int	ft_make_h_doc_loop(char *delimiter, \
	int here_doc_fd, t_detower *dll_envp_tower, int is_env_expand)
{
	char	*buffer;
	char	*tmp_buf;
	int		here_doc_stop;

	here_doc_stop = BOOL_FALSE;
	if (ft_make_h_doc_readline(&tmp_buf, &here_doc_stop) == FT_ERROR)
		return (FT_ERROR);
	if (here_doc_stop == BOOL_TRUE)
		return (free(tmp_buf), FT_SUCCESS);
	buffer = ft_strjoin(tmp_buf, "\n");
	free(tmp_buf);
	if (buffer == 0)
		return (FT_ERROR);
	if (ft_strcmp(buffer, delimiter) == 0)
		return (free(buffer), FT_SUCCESS);
	if (is_env_expand == BOOL_TRUE)
	{
		if (ft_token_str_expansion(&buffer, \
			dll_envp_tower->head, EXPAND_ENV_ONLY) == FT_ERROR)
			return (free(buffer), FT_ERROR);
	}
	if (write(here_doc_fd, buffer, ft_strlen(buffer)) < 0)
		return (free(buffer), FT_ERROR);
	return (free(buffer), -1);
}

int	ft_make_h_doc_readline(char **buffer, int *here_doc_stop)
{
	int	org_exit_code;
	int	ret;
	int	tmp_fd;

	org_exit_code = g_exit_code;
	g_exit_code = 0;
	tmp_fd = dup(STDIN_FILENO);
	ret = FT_SUCCESS;
	*buffer = readline("> ");
	if (*buffer == 0)
		ret = ft_make_h_doc_readline_eof_condition(\
				here_doc_stop, &org_exit_code, tmp_fd);
	g_exit_code = org_exit_code;
	return (ret);
}

int	ft_make_h_doc_readline_eof_condition(int *here_doc_stop, \
	int *org_exit_code, int tmp_fd)
{
	if (g_exit_code == 0)
	{
		*here_doc_stop = BOOL_TRUE;
		ft_putstr_fd("\x1b[1A", STDOUT_FILENO);
		ft_putstr_fd("\033[2C", STDOUT_FILENO);
		return (FT_SUCCESS);
	}
	else if (g_exit_code == 1)
	{
		*org_exit_code = 1;
		dup2(tmp_fd, STDIN_FILENO);
		close(tmp_fd);
		return (FT_ERROR);
	}
	return (FT_ERROR);
}