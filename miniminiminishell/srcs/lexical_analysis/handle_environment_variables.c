#include "../../includes/miniminiminishell.h"

static int	set_env_idx(char *str, int *dollar_idx, int *next_idx)
{
	*dollar_idx = 0;
	while (str[*dollar_idx] && str[*dollar_idx] != '$')
		*dollar_idx += 1;
	if (str[*dollar_idx] == '\0' || str[*dollar_idx + 1] == '\0')
		return (0);
	*next_idx = *dollar_idx + 1;
	if (str[*next_idx] == '?')
	{
		*next_idx += 1;
		return (1);
	}
	while (str[*next_idx] && (ft_isalnum(str[*next_idx]) || str[*next_idx] == '_'))
		*next_idx += 1;
	return (1);
}

static char	**get_exit_status_and_split(void)
{
	char	**strs;

	strs = malloc(sizeof(char *) * 4);
	if (!strs)
		return (NULL);
	strs[0] = ft_itoa(g_exit_status);
	strs[1] = ft_strdup(" ");
	strs[2] = ft_strdup("");
	strs[3] = 0;
	return (strs);
}

static char	**interpret_and_split_env(t_env_node *env_list, char *str, int dollar_idx, int next_idx)
{
	char	*env_value;
	char	**strs;
	int		space_idx;

	strs = malloc(sizeof(char *) * 4);
	if (!strs)
		return (NULL);
	env_value = ft_substr(str, dollar_idx + 1, next_idx - dollar_idx - 1);
	env_value = free_env_key_and_get_env_value(env_list, env_value);
	space_idx = 0;
	while (env_value[space_idx] && env_value[space_idx] != ' ')
		space_idx++;
	strs[0] = ft_substr(env_value, 0, space_idx);
	strs[1] = ft_strdup(" ");
	strs[2] = ft_substr(env_value, space_idx + 1, ft_strlen(env_value) - space_idx - 1);
	strs[3] = 0;
	free(env_value);
	return (strs);
}

static char	**seperate_environment_variables(t_info *info, char *str)
{
	char	**strs;
	char	**env_value;
	int		dollar_idx;
	int		next_idx;

	if (!set_env_idx(str, &dollar_idx, &next_idx)) // 환경변수가 없을때
		return (NULL);
	strs = malloc(sizeof(char *) * 6);
	if (!strs)
		return (NULL);
	strs[0] = ft_substr(str, 0, dollar_idx);
	if (str[dollar_idx + 1] == '?')
		env_value = get_exit_status_and_split();
	else
		env_value = interpret_and_split_env(info->env_list, str, dollar_idx, next_idx);
	strs[1] = ft_strdup(env_value[0]);
	strs[2] = ft_strdup(env_value[1]);
	strs[3] = ft_strdup(env_value[2]);
	free_2d_arr(env_value);
	strs[4] = ft_substr(str, next_idx, ft_strlen(str) - next_idx);
	strs[5] = 0;
	return (strs);
}

static void	convert_result_to_token(t_token *token_list, char **strs)
{
	t_token	*temp;

	temp = token_list->next;
	token_list->next = NULL;
	free(token_list->string);
	token_list->string = ft_strdup(strs[0]);
	add_token(&token_list, create_token(strs[1], TOKEN_TYPE_ARGV));
	add_token(&token_list, create_token(strs[2], TOKEN_TYPE_SPACE));
	add_token(&token_list, create_token(strs[3], TOKEN_TYPE_ARGV));
	token_list = add_token(&token_list, create_token(strs[4], TOKEN_TYPE_CHUNK));
	free_2d_arr(strs);
	token_list->next = temp;
}

void	handle_environment_variables(t_info *info, t_token *token_list)
{
	char	**strs;
	t_token	**head;

	head = &token_list;
	while (token_list)
	{
		// CHUNK => CHUNK, ARGV, SPACE, ARGV, CHUNK
		if (token_list->type == TOKEN_TYPE_CHUNK)
		{
			strs = seperate_environment_variables(info, token_list->string);
			if (strs)
				convert_result_to_token(token_list, strs);
		}
		token_list = token_list->next;
	}
}
