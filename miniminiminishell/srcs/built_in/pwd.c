#include "../../includes/miniminiminishell.h"

int ms_pwd(void)
{
	char current_working_dir[1024];

	if (getcwd(current_working_dir, 1024) == NULL)
	{
		ms_error("pwd", NULL);
		return (1);
	}
	printf("%s\n", current_working_dir);
	return (0);
}
