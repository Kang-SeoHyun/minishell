#include "../../includes/minishell.h"

static void	convert_result_to_token(t_token *token_list, char **strs)
{
	t_token	*temp;

	temp = token_list->next;
	token_list->next = NULL;
	free(token_list->string);
	token_list->string = ft_strdup(strs[0]);
	add_token(&token_list, create_token(strs[1], TOKEN_TYPE_ARGV));
	token_list = add_token(&token_list, create_token(strs[2],
				TOKEN_TYPE_CHUNK));
	free_2d_arr(strs);
	token_list->next = temp;
}

// CHUNK => CHUNK, ARGV, CHUNK
void	handle_quotes(t_info *info, t_token *token_list)
{
	char	**strs;

	while (token_list)
	{
		if (token_list->type == TOKEN_TYPE_CHUNK)
		{
			strs = seperate_quotes(info, token_list->string);
			if (strs)
				convert_result_to_token(token_list, strs);
		}
		token_list = token_list->next;
	}
}
