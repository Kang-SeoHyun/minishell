#include "../../includes/miniminiminishell.h"

static int	check_pipeline_syntax(t_token *token_list)
{
	t_token *prev;

	prev = NULL;
	while (token_list)
	{
		if (token_list->type == TOKEN_TYPE_PIPELINE)
			if (prev == NULL || token_list->next == NULL || token_list->next->type == TOKEN_TYPE_PIPELINE)
			{
				printf("miniminiminishell: syntax error near unexpected token \'|\'\n");
				return (1);
			}
		prev = token_list;
		token_list = token_list->next;
	}
	return (0);
}

static int check_redirection_syntax(t_token *token_list)
{
	while (token_list)
	{
		if (token_list->type == TOKEN_TYPE_REDIRECTION)
		{
			if (token_list->next == NULL || token_list->next->type != TOKEN_TYPE_ARGV)
			{
				printf("miniminiminishell: syntax error near unexpected token \'newline\'\n");
				return (1);
			}
		}
		token_list = token_list->next;
	}
	return (0);
}

int	syntax_analysis(t_token *token_list)
{
	// 1. pipeline 앞뒤는 Null 안됨
	// 2. redirection 뒤는 argv
	return (check_pipeline_syntax(token_list) 
		|| check_redirection_syntax(token_list));
}
