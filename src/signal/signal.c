/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kshim <kshim@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/23 18:24:01 by mikim3            #+#    #+#             */
/*   Updated: 2023/01/03 18:33:17 by kshim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_minishell.h"

void	set_signal(int sig_int, int sig_quit)
{
	if (sig_int == SIG_IGNORE)
		signal(SIGINT, SIG_IGN);
	if (sig_int == SIG_DEFAULT)
		signal(SIGINT, SIG_DFL); 
	if (sig_int == SIG_HANDLER)
		signal(SIGINT, signal_handler);   
	if (sig_int == SIG_CHILD_HANDLER)
		signal(SIGINT, signal_handler2);
	if (sig_quit == SIG_IGNORE)
		signal(SIGQUIT, SIG_IGN);
	if (sig_quit == SIG_DEFAULT)
		signal(SIGQUIT, SIG_DFL);
	if (sig_quit == SIG_HANDLER)
		signal(SIGQUIT, signal_handler);
	if (sig_quit == SIG_CHILD_HANDLER)
		signal(SIGQUIT, signal_handler2);
}

void	signal_handler(int signo)
{
	// ft_putstr_fd("\n\n\n\nSIG_1_HANDER before \n\n\n\n",STDERR_FILENO);
	if (signo == SIGINT) // Ctrl + c
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

// 자식
void	signal_handler2(int signo)
{

}

void	wait_child(void)
{
	int		status;
	int		signo;
	int		repeat;

	repeat = 0;
	while (wait(&status) != -1)
	{	
		//WIFSIGNALED 시그널에 의해 종료되었는지 확인
		if (WIFSIGNALED(status))
		{
			//WTERMSIG  어떤 시그에 의해 종료되었는지 확인
			signo = WTERMSIG(status);
			if (signo == SIGINT && repeat++ == 0)
				ft_putstr_fd("^C\n", STDERR_FILENO);
			else if (signo == SIGQUIT && repeat++ == 0)
				ft_putstr_fd("^\\Quit: 3\n", STDERR_FILENO);
			g_exit_code = 128 + signo;
		}
		else
			g_exit_code = WEXITSTATUS(status);
	}
}

