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

int	ms_export(t_info *info, char **argv)
{
	int			idx;
	t_env_node	*env_node;
	
	idx = 0;
	while (argv[++idx])
	{
		env_node = create_env_node(argv[idx]);
		if (ft_isdigit(env_node->key[0]))
		{
			printf("minishell: export: \'%s\': not a valid identifier\n", env_node->key); //printf한것들 표준출력으로 출력해주는걸로 바꿔야함
			free_env_list(env_node);
		}
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
	return (0);
}
