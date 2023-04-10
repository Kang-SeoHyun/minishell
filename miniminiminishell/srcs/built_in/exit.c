#include "../../includes/miniminiminishell.h"

// readline 반환값 input이 free되어야 하는지 .. 나중에 CHECK CHECK !!!
int	ms_exit(t_info *info, t_cmd *cmd_list)
{
	free_env_list(info->env_list);
	free_2d_arr(info->path_list);
	// free(info->home_path);
	if (cmd_list)
	{
		if (cmd_list->prev == NULL && cmd_list->next == NULL)
			printf("exit\n");
		free_cmd_list(&cmd_list);
	}
	else
		printf("exit\n");
	exit(0);
	return (0);
}
