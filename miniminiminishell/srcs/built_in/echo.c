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
