#include "../../includes/minishell.h"

int	ms_pwd(void)
{
	char	current_working_dir[1024];

	if (getcwd(current_working_dir, 1024) == NULL)
	{
		ms_error("pwd", NULL);
		return (1);
	}
	ft_putendl_fd(current_working_dir, STDOUT_FILENO);
	return (0);
}
