#include "../../includes/miniminiminishell.h"

void	ms_error(char *blame)
{
	ft_putstr_fd("minishell :", 2);
	ft_putstr_fd(blame, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
}

static void	signal_handler(int signum)
{
	if (signum == SIGINT)
		printf("🍄 minishell$ \n");
	rl_on_new_line();
	rl_replace_line("", 1);
	rl_redisplay();
}

void	set_signal(void)
{
	signal(SIGINT, signal_handler);	// ctrl + C
	signal(SIGQUIT, SIG_IGN);		// ctrl + "\"
}

int	is_tokenable_sep(char c)
{
	return (c == ' ' || c == '|' || c == '<' || c == '>' || c == '\0');
}

void	free_2d_arr(char **arr)
{
	int	i;

	i = -1;
	if (arr)
	{
		while (arr[++i])
			free(arr[i]);
		free(arr);
	}
}

void    free_strs(char *str1, char *str2, char *str3, char *str4)
{
	if (str1)
		free(str1);
	if (str2)
		free(str2);
	if (str3)
		free(str3);
	if (str4)
		free(str4);
}

char	*join_strs(char *str1, char *str2, char *str3)
{
	char	*str;
	int		size;
	int		i;

	size = ft_strlen(str1) + ft_strlen(str2) + ft_strlen(str3);
	str = malloc(sizeof(char) * (size + 1));
	if (!str)
		return (NULL);
	i = 0;
	if (str1)
		while (*str1)
			str[i++] = *str1++;
	if (str2)
		while (*str2)
			str[i++] = *str2++;
	if (str3)
		while (*str3)
			str[i++] = *str3++;
	str[size] = '\0';
	return (str);
}
