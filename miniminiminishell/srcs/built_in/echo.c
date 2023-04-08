#include "../../includes/miniminiminishell.h"

int	ms_echo(char **argv)
{
	int	idx;
	int n_idx;
	int	newline;

	newline = 1;
	idx = 0;
	if (argv[1] && ft_strncmp(argv[1], "-n", 2) == 0)
	{
		n_idx = 1;
		while (argv[1][n_idx] == 'n')
			n_idx++;
		if (argv[1][n_idx] == '\0')
			newline = 0;
		if (argv[1][n_idx] == '\0')
			idx++;
	}
	while (argv[++idx])
	{
		if (argv[idx][0] == '\0')
			continue ;
		printf("%s", argv[idx]);
		if (argv[idx + 1])
			printf(" ");
	}
	if (newline)
		printf("\n");
	return (0);
}
/*

1. single cmd => 빌트인 사용했으면 그 끝에;
	만약에 포크 했으면 wait 부모에서 받으면 됨 wait(info->status);
*/