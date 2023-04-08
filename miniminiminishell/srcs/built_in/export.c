#include "../../includes/miniminiminishell.h"

static int	change_value_if_key_exist(t_info *info, char *key, char *value)
{
	t_env_node	*cur;

	cur = info->env_list;
	while (cur)
	{
		if (ft_strncmp(cur->key, key, ft_strlen(key)) == 0)
		{
			if (ft_strncmp(key, "PATH", 4) == 0)
			{
				free_2d_arr(info->path_list);
				info->path_list = ft_split(value, ':');
			}
			if (value)
			{
				if (cur->value)
					free(cur->value);
				cur->value = ft_strdup(value);
			}
			return (1);
		}
		cur = cur->next;
	}
	return (0);
}

static void	print_env_list(t_env_node *env_list)
{
	while(env_list)
	{
		printf("declare -x %s", env_list->key);
		if (env_list->value)
			printf("=\"%s\"", env_list->value);
		printf("\n");
		env_list = env_list->next;
	}
}

static int	set_error_flag(t_env_node *env_node, char *str, int *error_flag)
{
	if (ft_isdigit(str[0]))
	{
		ft_putstr_fd("minishell: export: \'", STDERR_FILENO);
		ft_putstr_fd(str, STDERR_FILENO);
		ft_putstr_fd("\': not a valid identifier\n", STDERR_FILENO);
		free_env_list(env_node);
		*error_flag = 1;
		return (1);
	}
	return (0);
}

int	ms_export(t_info *info, char **argv)
{
	int			idx;
	t_env_node	*env_node;
	int			error_flag;
	
	idx = 0;
	error_flag = 0;
	while (argv[++idx])
	{
		env_node = create_env_node(argv[idx]);
		if (set_error_flag(env_node, argv[idx], &error_flag))
			continue ;
		else if (change_value_if_key_exist(info, env_node->key, env_node->value))
			free_env_list(env_node);
		else // 기존에 없던 환경변수면 추가
		{
			if (ft_strncmp(env_node->key, "PATH", 4) == 0)
				info->path_list = ft_split(env_node->value, ':');
			add_env_node(&info->env_list, env_node);
		}
	}
	if (idx == 1)
		print_env_list(info->env_list);
	return (error_flag);
}
