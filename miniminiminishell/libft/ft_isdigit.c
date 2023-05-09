#include "libft.h"

/**
 * The ft_isdigit() function tests for a decimal digit character.
 */
int	ft_isdigit(int c)
{
	if ('0' <= c && c <= '9')
		return (1);
	else
		return (0);
}
