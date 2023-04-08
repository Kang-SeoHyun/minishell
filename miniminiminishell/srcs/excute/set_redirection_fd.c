#include "../../includes/miniminiminishell.h"

static int	get_infile_fd(char *infile)
{
	int	fd;

	fd = open(infile, O_RDONLY);
	if (fd == -1 || dup2(fd, STDIN_FILENO) == -1)
	{
		ms_error(infile);
		return (-1);
	}
	return (fd);
}

static int	ms_heredoc(char *limiter)
{
	int		fd;
	char	*input;

	fd = open("heredoc.tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (-1);
	limiter = ft_strjoin(limiter, "\n");
	while (1)
	{
		ft_putstr_fd("> ", STDOUT_FILENO);
		input = get_next_line(STDIN_FILENO);
		if (input == NULL || ft_strncmp(input, limiter, ft_strlen(input)) == 0) //나중에 히어독 체크
			break ;
		write(fd, input, ft_strlen(input));
		free(input);
		input = NULL;////////////////////
	}
	close(fd);
	free(limiter);
	free(input);
	return (open("heredoc.tmp", O_RDONLY));
}

static int	get_heredoc_fd(char *limiter)
{
	int	fd;
	fd = ms_heredoc(limiter);
	if (fd == -1 || dup2(fd, STDIN_FILENO) == -1)
	{
		ms_error("heredoc");
		return (-1);
	}
	return (fd);
}

static int	get_overwrite_fd(char *outfile)
{
	int	fd;
	
	fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1 || dup2(fd, STDOUT_FILENO) == -1)
	{
		ms_error(outfile);
		return (-1);
	}
	return (fd);
}

static int	get_append_fd(char *outfile)
{
	int	fd;
	
	fd = open(outfile, O_RDWR | O_CREAT | O_APPEND, 0644);
	if (fd == -1 || dup2(fd, STDOUT_FILENO) == -1)
	{
		ms_error(outfile);
		return (-1);
	}
	return (fd);
}

int	set_redirection_fd(t_cmd *cmd_list)
{
	int	fd;

	while (cmd_list->redirection)
	{
		if (ft_strncmp(cmd_list->redirection->type, "<<", 2) == 0)
			fd = get_heredoc_fd(cmd_list->redirection->file);
		else if (ft_strncmp(cmd_list->redirection->type, "<", 1) == 0)
			fd = get_infile_fd(cmd_list->redirection->file);
		else if (ft_strncmp(cmd_list->redirection->type, ">>", 2) == 0)
			fd = get_append_fd(cmd_list->redirection->file);
		else if (ft_strncmp(cmd_list->redirection->type, ">", 1) == 0)
			fd = get_overwrite_fd(cmd_list->redirection->file);
		if (fd == -1)
			return (-1);
		cmd_list->redirection = cmd_list->redirection->next;
	}
	return (0);
}