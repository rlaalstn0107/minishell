/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mikim3 <mikim3@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/26 00:49:10 by mikim3            #+#    #+#             */
/*   Updated: 2022/12/28 15:51:00 by mikim3           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_minishell.h"

void execute_cmd(t_tree_node *token_tree, t_detower *dll_envp_tower, t_pipe *m_pipe)
{
    printf("in the execute_cmd\n");
	printf("(token_tree->content)->cmd_name  %s \n",((t_tree_cmd *)token_tree->content)->cmd_name);

	if (execute_noprint_builtin(token_tree->content, dll_envp_tower, m_pipe) == 1)
		return ;
	// 출력을 하려면  포크를 해야 됨
    execute_fork(token_tree,dll_envp_tower,m_pipe); 
}

void	execute_fork(t_tree_node *token_tree, t_detower *dll_envp_tower, t_pipe *m_pipe)
{
	pid_t	pid;
	pid_t	w_pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		//파이프 관련처리
		if (is_built_in(token_tree->content))
			execute_print_builtin(token_tree->content, dll_envp_tower, m_pipe);
		else
			execute_external(token_tree, dll_envp_tower, m_pipe);
	}
	else if (pid > 0) // 부모
	{
		//자식 프로세스 기다리기, 파이프처리

		wait_child();
	}
	else //에러 출력
		printf("pid error\n");
}

int	is_built_in(t_tree_cmd *cmd)
{
	if (!ft_strcmp(cmd->cmd_name, "cd"))
		return (1);
	if (!ft_strcmp(cmd->cmd_name, "env"))
		return (1);
	if (!ft_strcmp(cmd->cmd_name, "export"))
		return (1);
	if (!ft_strcmp(cmd->cmd_name, "unset"))
		return (1);
	if (!ft_strcmp(cmd->cmd_name, "pwd"))
		return (1);
	if (!ft_strcmp(cmd->cmd_name, "echo"))
		return (1);
	if (!ft_strcmp(cmd->cmd_name, "exit"))
		return (1);
	return (0);
}

void	execute_print_builtin(t_tree_cmd *cmd, t_detower *dll_envp_tower, t_pipe *m_pipe)
{
	if (!ft_strcmp(cmd->cmd_name, "env"))
		// ft_env(cmd, dll_envp_tower, m_pipe);
	if (!ft_strcmp(cmd->cmd_name, "export"))
		// ft_export(cmd, dll_envp_tower, m_pipe);
	if (!ft_strcmp(cmd->cmd_name, "pwd"))
		// ft_pwd(cmd, dll_envp_tower, m_pipe);
	if (!ft_strcmp(cmd->cmd_name, "echo"))
		// ft_echo(cmd, dll_envp_tower, m_pipe);
	return ;
}

// env에 PATH 안에 명령어를 찾아서 그 경로를 반환
char	**get_env_path(t_detower *dll_envp_tower)
{
	t_d_list	*node;
	int			index;
	char		**env_path_value;
	int			loc;

	// char		*tmp; // key값 value값 분리 안되어 있을때 사용
	// char		*tmp2;

	printf("get_env_path() \n");
	// t_detower의 
	node = dll_envp_tower->head;
	index = 0;
	// 밖에서 key , value 나뉘어서 온다면 이거대로 쓰기
	while (node != NULL)
	{
		printf(" ((t_envp_content *)node->content)->key == %s\n",((t_envp_content *)node->content)->key);
		printf(" ((t_envp_content *)node->content)->value == %s\n",((t_envp_content *)node->content)->value);

		if(ft_strcmp(((t_envp_content *)node->content)->key, "PATH"))
		{
			printf("find PATH in the get_env_path() \n");
			env_path_value = ft_split(((t_envp_content *)node->content)->value, ':');
		}
		node = node->next;
	}

	// 키 벨류 여기서 나눌까 하다가 건혁님이 이미 하고 있으셔서 보류
 	// while (node != NULL)
	// {

	// 	tmp = ft_strchr_and_loc(((t_envp_content *)node->content)->key,'=',&loc);
	// 	tmp2 = ft_substr(tmp,0,loc);
	// 	printf(" tmp2 == %s\n",tmp2);
	// 	printf(" tmp == %s\n",tmp + 1);

	// 	if(ft_strcmp(((t_envp_content *)node->content)->key, "PATH"))
	// 	{
	// 		env_path_value = ft_split(((t_envp_content *)node->content)->value, ':');
	// 		printf("find PATH in the get_env_path() \n");
	// 	}
	// 	node = node->next;
	// }
	return (env_path_value);
}

void	double_char_free(char **double_char)
{
	int	i;

	i = 0;
	while(double_char[i])
		free(double_char[i]);
	free(double_char);
}

char	*get_file_path_from_env_path(char *command,t_detower *dll_envp_tower)
{
	char	*file_path;
	char	**env_path_values;
	char	*tmp;
	int		index;

	file_path = NULL;
	// key가 PATH인 환경변수의 value값을 split(:)으로 나누어서 env_path_values에 저장
	env_path_values = get_env_path(dll_envp_tower);
	
	index = 0;
	while(env_path_values[index])
	{
		tmp = ft_strjoin(ft_strdup("/"), ft_strdup(command));
		file_path = ft_strjoin(ft_strdup(env_path_values[index]), tmp);
		//존재하지 않으면 free(), file_path에 NULL값 넣기후 다음 인덱스
		// if()
		// 실행권한 없음
		if (access(file_path,X_OK) == -1)
		{
			free(file_path);
			file_path = NULL;
		}
		else
			break ;
		index++;
	}
	if (env_path_values != NULL)  
		// 이차원배열 free 있는지 찾아보고 없으면 함수 만들기
		double_char_free(env_path_values);
	return	(file_path);
}

char	*get_current_path(void)
{
	char	*current_path;

	current_path = getcwd(NULL, 0); //현재 경로 
	return (current_path);
}

char	*set_file_path(char *command, t_detower *dll_envp_tower)
{
	char	*file_path;
	char	*current_path;

	printf("in the set_file_path()\n");
	if (ft_strncmp(command, "/", 1) == 0) // 절대경로는 그대로 준다.
	{
		file_path = ft_strdup(command);
	}
	else if (ft_strncmp("./", command, 2) == 0) // 상대경로 ./bash_exe_ex hahaha 현재경로에 bash를 실행한다고 생각  
	{
		command = ft_substr(command, 2, ft_strlen(command) - 2);
		current_path = get_current_path();
		printf("current_path == %s \n",current_path);
		file_path = ft_strjoin(current_path, ft_strdup("/"));
		printf("file_path == %s \n",file_path);
		file_path = ft_strjoin(file_path, command);
		printf("file_path == %s \n",file_path);
	}
	else // 환경변수 PATH에 세팅되어서 명령어로 실행할수 있는 명령어
	{
		// PATH=/Users/mikim3/brew/bin:/Users/mikim3/goinfre/brew/bin:/usr/local/bin 
		// :/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/munki
		// (:)콜론으로 구분된다.
		// 절대경로로 PATH안에 있는 값들에 해당하는 폴더까지 간 다음에 해당 명령어에 해당하는 파일이 있는지 확인
		// 없으면 다음 : 확인 반복
		printf("in the else\n");
		file_path = get_file_path_from_env_path(command, dll_envp_tower);
	}
	return (file_path);
}

// 외부함수 실제 실행
void	execute_external(t_tree_node *node,t_detower *dll_envp_tower,t_pipe *m_pipe)
{
	extern	char	**environ;
	char			*file_path;

	printf("in the execute_external \n");

	// file_path를 여기서만 쓰기

	file_path = set_file_path(((t_tree_cmd *)node->content)->cmd_name,dll_envp_tower);
	printf("file_path in execute_external file_path = %s \n");
	// t_tree_cmd


	printf("node->content)->cmd_name %s\n",((t_tree_cmd *)node->content)->cmd_name);
	// printf("node->content)->file_path %s\n",((t_tree_cmd *)node->content)->file_path);
	int i = 0;
	while (((t_tree_cmd *)node->content)->cmd_argv[i] != 0)
	{
		printf("node->content)->argv %s\n",((t_tree_cmd *)node->content)->cmd_argv[i]);
		i++;
	}


	// dll_envp_tower 바뀐 환경변수를 넣어야 될것 같은데 일단은 이렇게 만듬
	// 실행 성공시에는 리턴을 받을수가 없다.
	// if (execve((file_path,((t_tree_cmd *)node->content)->cmd_argv, environ)== -1))
	if (execve(file_path,((t_tree_cmd *)node->content)->cmd_argv, environ)== -1)
	{
		//execve실패 명령어를 못찾은 상태 
		printf("execve 실패 \n");
	}
	//필요한지 다시 생각해보기
	free(file_path);
}

// 프린트 안하는 빌트인 함수 실행
int		execute_noprint_builtin(t_tree_cmd *cmd, t_detower *dll_envp_tower,t_pipe *m_pipe)
{
	printf("execute_noprint_builtin\n");
	if (cmd->cmd_name == NULL)
	{
		printf("cmd->cmd_name == NULL \n");
	}
	if (!ft_strcmp(cmd->cmd_name, "exit"))
	{
		// ft_exit(cmd,dll_envp_tower,m_pipe);
		return (1);
	}
	if (!ft_strcmp(cmd->cmd_name, "unset"))
	{
		// ft_unset(cmd,dll_envp_tower,m_pipe);
		return (1);
	}
	if (!ft_strcmp(cmd->cmd_name, "cd"))
	{
		// ft_cd(cmd,dll_envp_tower,m_pipe);
		return (1);
	}
	// export가 인자가 있으면 출력을 해야만함
	if (!ft_strcmp(cmd->cmd_name, "export") && cmd->cmd_argv[1] != NULL)
	{
		// ft_export(cmd,dll_envp_tower,m_pipe);
		return (1);
	}
	printf("execute_noprint_builtin don't execve and finish\n");
	return (0);
}



// 
void	wait_child(void)
{
	int		status;
	int		signo;
	int		i;

	i = 0;
	while (wait(&status) != -1)
	{
		if (WIFSIGNALED(status))
		{
			signo = WTERMSIG(status);
			if (signo == SIGINT && i++ == 0)
				ft_putstr_fd("^C\n", STDERR_FILENO);
			else if (signo == SIGQUIT && i++ == 0)
				ft_putstr_fd("^\\Quit: 3\n", STDERR_FILENO);
			g_exit_code = 128 + signo;
		}
		else
			g_exit_code = WEXITSTATUS(status);
	}

	
}