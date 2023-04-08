#include "../../includes/miniminiminishell.h"

static t_env_node	*delete_env_list(t_env_node *env_list)
{
	t_env_node	*next;

	next = env_list->next;
	free(env_list->key);
	free(env_list->value);
	free(env_list);
	return (next);
}

int	ms_unset(t_info *info, char **argv)
{
	t_env_node *cur;
	t_env_node *prev;
	int			idx;

	idx = 0;
	while (argv[++idx])
	{
	    cur = info->env_list;
		prev = NULL;
		while (cur)
		{
			if (ft_strncmp(cur->key, argv[idx], ft_strlen(argv[idx])) == 0)
			{
				if (ft_strncmp(cur->key, "PATH", 4) == 0)
					free_2d_arr(info->path_list);
				if (prev == NULL)
					info->env_list = delete_env_list(cur);
				else
					prev->next = delete_env_list(cur);
				break ;
			}
			prev = cur;
			cur = cur->next;
		}
	}
	return (0);
}
