#include "../../includes/minishell.h"

void	print_token_list(t_token *token_list)
{
	while (token_list)
	{
		if (token_list->type == TOKEN_TYPE_CHUNK)
			printf(GREEN"{%s}"RESET, token_list->string);
		else if (token_list->type == TOKEN_TYPE_ARGV)
			printf(MAGENTA"[%s]"RESET, token_list->string);
		else if (token_list->type == TOKEN_TYPE_SPACE)
			printf(BLUE"[%s]"RESET, token_list->string);
		else if (token_list->type == TOKEN_TYPE_PIPELINE)
			printf(YELLOW"[%s]"RESET, token_list->string);
		else if (token_list->type == TOKEN_TYPE_REDIRECTION)
			printf(RED"[%s]"RESET, token_list->string);
		token_list = token_list->next;
	}
	printf("\n");
}

void	print_cmd_list(t_cmd *cmd_list)
{
	int	i;
	t_redirection	*cur;

	while (cmd_list)
	{
		if (cmd_list->argv)
		{
			printf("argv = ");
			i = -1;
			while (cmd_list->argv[++i])
				printf("[%s] ", cmd_list->argv[i]);
			printf("\n");
		}
		cur = cmd_list->redirection;
		while (cur)
		{
			printf("redirection = t: %s, f: %s\n", cur->type, cur->file);////
			cur = cur->next;
		}
		cmd_list = cmd_list->next;
	}
}


t_token	*lexical_analysis(t_info *info, char *input)
{
	t_token	*token_list;

	// 1. input -> chunk
	token_list = create_token(input, TOKEN_TYPE_CHUNK);
	printf("=== 1 최초의 CHUNK가 있었도다 ===================\n");
	print_token_list(token_list);

	// 2. handle heredoc limiter
	handle_heredoc_limiter(info, token_list);
	printf("=== 2 heredoc limiter 처리 ======================\n");
	print_token_list(token_list);

	// 3. handle quotes
	handle_quotes(info, token_list);
	printf("=== 3 따옴표 처리 ===============================\n");
	print_token_list(token_list);

	// 4. handle environment variables
	handle_environment_variables(info, token_list);
	printf("=== 4 환경 변수 처리 ============================\n");
	print_token_list(token_list);
	
	// 5. seperate space token
	seperate_token_by_arg(token_list, " ");
	printf("=== 5 SPACE 분리 ================================\n");
	print_token_list(token_list);

	// 6. seperate pipeline token
	seperate_token_by_arg(token_list, "|");
	printf("=== 6 PIPELINE 분리 =============================\n");
	print_token_list(token_list);

	// 7. seperate redirection toekn
	seperate_token_by_arg(token_list, "<<");
	seperate_token_by_arg(token_list, ">>");
	seperate_token_by_arg(token_list, "<");
	seperate_token_by_arg(token_list, ">");
	printf("=== 7 REDIRECTION 분리 ==========================\n");
	print_token_list(token_list);

	// 8. remove empty chunk & change chunk to argv
	handle_chunk(&token_list);
	printf("=== 8 CHUNK 처리 ================================\n");
	print_token_list(token_list);
	
	// 9. merge continuous argv token
	merge_continuous_argv_token(&token_list);
	printf("=== 9 연속된 argv 병합 ==========================\n");
	print_token_list(token_list);

	// 10. remove space token
	remove_space_token(&token_list);
	printf("=== 10 SPACE 지우기 =============================\n");
	print_token_list(token_list);
	
	printf("\n");
	return (token_list);
}
