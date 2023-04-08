#include "../includes/miniminiminishell.h"

static void	initialize(t_info *info_ptr, char **envp)
{
	info_ptr->stdin = dup(STDIN_FILENO);
	info_ptr->stdout = dup(STDOUT_FILENO);
	tcgetattr(STDIN_FILENO, &(info_ptr->ms_termios));
	(&info_ptr->ms_termios)->c_lflag &= ~(ECHOCTL);
	tcsetattr(STDIN_FILENO, TCSANOW, &(info_ptr->ms_termios));
	info_ptr->env_list = init_env_list(envp);
	while (ft_strncmp(*envp, "PATH=", 5))
		envp++;
	info_ptr->path_list = ft_split(*envp + 5, ':');
	info_ptr->home_path = free_env_key_and_get_env_value(info_ptr->env_list, ft_strdup("HOME"));
}

void	print_token_list(t_token *token_list)///////////////
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
			printf("redirection = t: %s, f: %s\n", cur->type, cur->file);
			cur = cur->next;
		}
		cmd_list = cmd_list->next;
	}
}

void	run_minishell(t_info *info)
{
	t_token	*token_list;
	t_cmd	*cmd_list;
	char	*input;
	printf("엿같은 미치광이 쉘이 시작됐어용\n");
	while (1)
	{
		set_signal();
		input = readline("🍄 minishell$ ");
		if (!input) // ctrl + D
		{
			printf("\033[1A");
			printf("\033[14C");
			ms_exit(info, cmd_list);
		}
		else if (*input != '\0')
		{
			add_history(input);
			info->syntax_error = 0;
			token_list = lexical_analysis(info, input);
			if (!info->syntax_error)
				info->syntax_error = syntax_analysis(token_list);
			if (!info->syntax_error)
				cmd_list = create_cmd_list(token_list);
			free_token_list(token_list);
			if (cmd_list)
			{
				ms_execute(info, cmd_list);
				free_cmd_list(&cmd_list);
			}
		}
		free(input);
	}
}

void	check_leak(void)
{
	system("leaks miniminiminishell");
}

int	main(int argc, char **argv, char **envp)
{
	t_info	info;

	(void)argc;
	(void)argv;
	// atexit(check_leak);
	initialize(&info, envp);
	run_minishell(&info);
	return (0);
}
