#include "../../includes/miniminiminishell.h"

char	*get_cmd_file(char *cmd, char **path_list)
{
	char	*cmd_file;
	char	*temp;

	if (cmd == NULL)
		return (NULL);
	if (access(cmd, X_OK) == 0)
		return (cmd);
	temp = ft_strjoin("/", cmd);
	while (*path_list)
	{
		cmd_file = ft_strjoin(*path_list, temp);
		if (access(cmd_file, X_OK) == 0)
		{
			free(temp);
			return (cmd_file);
		}
		free(cmd_file);
		path_list++;
	}
	free(temp);
	return (NULL);
}


void	set_io_fd(t_info *info, t_cmd *cmd_list)
{
	if (cmd_list->prev)
	{
		if (dup2(cmd_list->prev->pipe[0], STDIN_FILENO) == -1)
		{
			info->exit_status = errno; ////////////////
			exit(info->exit_status);
		}
	}
	if (cmd_list->next)
	{
		if (dup2(cmd_list->pipe[1], STDOUT_FILENO) == -1)
		{
			info->exit_status = errno; ////////////////
			exit(info->exit_status);
		}
	}
	if (set_redirection_fd(cmd_list) == -1)
	{
		info->exit_status = errno; ////////////////
		exit(info->exit_status);
	}
}

// void	execute_cmd(t_info *info, t_cmd *cmd_list)
// {
// 	char	*file;

// 	if (check_builtin(argv[0]))
// 		execute_builtin(info, cmd_list);
// 	file = get_cmd_file(cmd_list->argv[0], cmd_list->path_list);
// 	if (file == NULL)
// 	{
// 		printf("minishell: %s: command not found\n", cmd_list->argv[0]);
// 		info->exit_status = 127;
// 		exit(info->exit_status);
// 	}
// 	set_io_fd(cmd_list);//
// 	execve(file, cmd_list->argv, env_list_to_envp(info->env_list));
// 	// error	
// }

int	check_builtin(t_info *info, t_cmd *cmd_list) // 성공하면 0, 실패하면 -1, 빌트인 아니면 1
{
	if (ft_strncmp(cmd_list->argv[0], "cd", 2) == 0)
		return (ms_cd(info, cmd_list->argv));
	if (ft_strncmp(cmd_list->argv[0], "echo", 4) == 0)
		return (ms_echo(cmd_list->argv));
	if (ft_strncmp(cmd_list->argv[0], "env", 3) == 0)
		return (ms_env(info->env_list));
	if (ft_strncmp(cmd_list->argv[0], "exit", 4) == 0)
		return (ms_exit(info, cmd_list));
	if (ft_strncmp(cmd_list->argv[0], "export", 6) == 0)
		return (ms_export(info, cmd_list->argv));
	if (ft_strncmp(cmd_list->argv[0], "pwd", 3) == 0)
		return (ms_pwd());
	if (ft_strncmp(cmd_list->argv[0], "unset", 5) == 0)
		return (ms_unset(info, cmd_list->argv));
	return (1);
}

void	execute_single_cmd(t_info *info, t_cmd *cmd_list)
{
	char	*file;
	pid_t	pid;

	if (set_redirection_fd(cmd_list) == -1)
	{
		info->exit_status = errno;
		return ;
	}
	if (cmd_list->argv == NULL)
		return ;
	if (check_builtin(info, cmd_list) == 1)
	{
		printf("이자식아: %s \n",cmd_list->argv[0]);
		printf("엥...?ㅜㅜ\n");
		pid = fork();
		if (pid < 0)
			ms_error("fork");
		if (pid == 0)
		{
			file = get_cmd_file(cmd_list->argv[0], info->path_list);
			if (file == NULL)
			{
				printf("minishell: %s: command not found\n", cmd_list->argv[0]);
				info->exit_status = 127;
				return ;
			}
			execve(file, cmd_list->argv, env_list_to_envp(info->env_list));
		}
		else
			waitpid(pid, NULL, 0);
	}
}

void	ms_execute(t_info *info, t_cmd *cmd_list)
{
	// if (cmd_list->next)
	// 	execute_multiple_cmd(info, cmd_list);
	// else
	// 	execute_single_cmd(info, cmd_list);
	execute_single_cmd(info, cmd_list);
}
