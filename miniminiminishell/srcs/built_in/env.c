#include "../../includes/miniminiminishell.h"

int	ms_env(t_env_node *env_list)
{
	while(env_list)
	{
		if (env_list->value)
			printf("%s=%s\n", env_list->key, env_list->value);
		env_list = env_list->next;
	}
	return (0);
}
