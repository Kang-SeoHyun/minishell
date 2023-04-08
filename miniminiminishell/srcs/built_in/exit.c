#include "../../includes/miniminiminishell.h"

// readline 반환값 input이 free되어야 하는지 .. 나중에 CHECK CHECK !!!
int	ms_exit(t_info *info, t_cmd *cmd_list)
{
	free_env_list(info->env_list);
	free_2d_arr(info->path_list);
	free(info->home_path);
	if (cmd_list)
		free_cmd_list(&cmd_list);
	printf("exit\n");
	exit(0);
	return (0);
}
