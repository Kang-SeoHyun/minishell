#include "../../includes/minishell.h"

int	ms_env(t_env_node *env_list)
{
	while (env_list)
	{
		if (env_list->value)
		{
			ft_putstr_fd(env_list->key, STDOUT_FILENO);
			ft_putstr_fd("=", STDOUT_FILENO);
			ft_putendl_fd(env_list->value, STDOUT_FILENO);
		}
		env_list = env_list->next;
	}
	return (0);
}
