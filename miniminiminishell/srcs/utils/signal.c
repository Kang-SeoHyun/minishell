#include "../../includes/minishell.h"

void	signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		ft_putendl_fd(CYAN"💎 minishell-1.0$ "RESET, STDOUT_FILENO);
		g_exit_status = 1;
		rl_on_new_line();
		rl_replace_line("", 1);
		rl_redisplay();
	}
}

void	parent_handler(int signum)
{
	if (signum == SIGINT)
		ft_putendl_fd("", STDERR_FILENO);
}

void	heredoc_handler(int signum)
{
	if (signum == SIGINT)
	{
		rl_on_new_line();
		rl_replace_line("", 1);
		rl_redisplay();
		ft_putendl_fd("", STDERR_FILENO);
		exit(1);
	}
}

void	quit_handler(int signum)
{
	if (signum == SIGINT)
		ft_putendl_fd("", STDERR_FILENO);
	exit(1);
}

void	set_signal(void)
{
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
}
