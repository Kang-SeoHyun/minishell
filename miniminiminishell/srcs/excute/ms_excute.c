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


void	set_io_fd(t_cmd *cmd_list)
{
	if (cmd_list->prev)
	{
		if (dup2(cmd_list->prev->pipe[0], STDIN_FILENO) == -1)
		{
			g_exit_status = errno; ////////////////
			exit(g_exit_status);
		}
	}
	if (cmd_list->next)
	{
		if (dup2(cmd_list->pipe[1], STDOUT_FILENO) == -1)
		{
			g_exit_status = errno; ////////////////
			exit(g_exit_status);
		}
	}
	if (set_redirection_fd(cmd_list) == -1)
	{
		g_exit_status = errno; ////////////////
		exit(g_exit_status);
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
// 		g_exit_status = 127;
// 		exit(g_exit_status);
// 	}
// 	set_io_fd(cmd_list);//
// 	execve(file, cmd_list->argv, env_list_to_envp(info->env_list));
// 	// error	
// }

int	execute_builtin(t_info *info, t_cmd *cmd_list)
{
	if (set_redirection_fd(cmd_list) == -1)
	{
		g_exit_status = 1;
		return (0);
	}
	dup2(info->stdin, STDIN_FILENO);
	if (ft_strncmp(cmd_list->argv[0], "cd", 2) == 0)
		g_exit_status = ms_cd(info, cmd_list->argv);
	if (ft_strncmp(cmd_list->argv[0], "echo", 4) == 0)
		g_exit_status = ms_echo(cmd_list->argv);
	if (ft_strncmp(cmd_list->argv[0], "env", 3) == 0)
		g_exit_status = ms_env(info->env_list);
	if (ft_strncmp(cmd_list->argv[0], "exit", 4) == 0)
		g_exit_status = ms_exit(info, cmd_list);
	if (ft_strncmp(cmd_list->argv[0], "export", 6) == 0)
		g_exit_status = ms_export(info, cmd_list->argv);
	if (ft_strncmp(cmd_list->argv[0], "pwd", 3) == 0)
		g_exit_status = ms_pwd();
	if (ft_strncmp(cmd_list->argv[0], "unset", 5) == 0)
		g_exit_status = ms_unset(info, cmd_list->argv);
	dup2(info->stdout, STDOUT_FILENO); ///////// 메인으로 옮겨야 할수동 ...ㅇㅋㅇㅋ췍췍
	return (1);
}

int	check_builtin(t_info *info, t_cmd *cmd_list)
{
	char	*cmd;

	if (cmd_list->argv == NULL)
		return (0);
	cmd = cmd_list->argv[0];
	if ((ft_strlen(cmd) == 2 && ft_strncmp(cmd, "cd", 2) == 0) \
		|| (ft_strlen(cmd) == 4 && ft_strncmp(cmd, "echo", 4) == 0) \
		|| (ft_strlen(cmd) == 3 && ft_strncmp(cmd, "env", 3) == 0) \
		|| (ft_strlen(cmd) == 4 && ft_strncmp(cmd, "exit", 4) == 0) \
		|| (ft_strlen(cmd) == 6 && ft_strncmp(cmd, "export", 6) == 0) \
		|| (ft_strlen(cmd) == 3 && ft_strncmp(cmd, "pwd", 3) == 0) \
		|| (ft_strlen(cmd) == 5 && ft_strncmp(cmd, "unset", 5) == 0))
		return (execute_builtin(info, cmd_list));
	return (0);
}

void	execute_single_cmd(t_info *info, t_cmd *cmd_list)
{
	char	*file;//
	pid_t	pid;

// Ctrl+d

// EOF 를의미
// 시그널이 아니다!.
// stdin pipe를 닫는다.(read(STDIN) return 0)


	if (!check_builtin(info, cmd_list))
	{
		pid = fork();
		if (pid < 0)
			ms_error("fork", NULL);
		if (pid == 0)
		{
			// signal(SIGINT, child_handler);
			if (set_redirection_fd(cmd_list) == -1)
			{
				g_exit_status = 1;
				exit(g_exit_status);
			}
			if (cmd_list->argv == NULL)
				exit(g_exit_status);
			file = get_cmd_file(cmd_list->argv[0], info->path_list);
			if (file == NULL)
			{
				ft_putstr_fd("minishell: ", STDERR_FILENO);
				ft_putstr_fd(cmd_list->argv[0], STDERR_FILENO);
				ft_putstr_fd(": command not found\n", STDERR_FILENO);
				g_exit_status = 127;
				exit(g_exit_status);
			}
			execve(file, cmd_list->argv, env_list_to_envp(info->env_list));
			ms_error("execve", NULL);
			g_exit_status = 1;
			exit(g_exit_status);
		}
		else
		{
			signal(SIGINT, parent_handler);
			wait(&(g_exit_status));
			if (WIFSIGNALED(g_exit_status)) // signal에 의하여 terminate 되었는지를 확인
				g_exit_status = 128 + WTERMSIG(g_exit_status); // WIFSIGNALED의 값이 참인 경우 어느 signal에 의하여 terminate 되었는지를 알아냄
			if (WIFEXITED(g_exit_status))
				g_exit_status = WEXITSTATUS(g_exit_status); // exit code 알아냄
		}
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
