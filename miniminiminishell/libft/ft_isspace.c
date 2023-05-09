#include "libft.h"

/**
 * The ft_isspace() function tests for the white-space characters.
 */
int	ft_isspace(int c)
{
	if (c == '\t' || c == '\n' || c == '\v'
		|| c == '\f' || c == '\r' || c == ' ')
		return (1);
	else
		return (0);
}
