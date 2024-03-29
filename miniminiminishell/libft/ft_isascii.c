#include "libft.h"

/**
 * The ft_isascii() function tests for an ASCII character, which is any character
 * between 0 and octal 0177 inclusive.
 */
int	ft_isascii(int c)
{
	if (0 <= c && c <= 127)
		return (1);
	else
		return (0);
}
