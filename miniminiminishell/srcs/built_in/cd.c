#include "../../includes/miniminiminishell.h"

int	ms_cd(t_info *info, char **argv)
{
	char	*path;

	if (argv[1] && ft_strncmp(argv[1], "~", 1) == 0)
		chdir(info->home_path);
	else if (argv[1] && (access(argv[1], F_OK) || chdir(argv[1])))
	{
		printf("minishell: cd: %s: No such file or directory\n", argv[1]);
		return (-1);
	}
	if (argv[1] == NULL)
	{
		path = free_env_key_and_get_env_value(info->env_list, ft_strdup("HOME"));
		if (path[0] == '\0')
		{
			free(path);
			printf("minishell: cd: HOME not set\n");
			return (-1);
		}
		chdir(path);
		free(path);
	}
	return (0);
}
/*

unset

cd ::  error

cd ~ 

*/
