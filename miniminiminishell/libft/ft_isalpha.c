#include "libft.h"

static int	ft_isupper(int c);
static int	ft_islower(int c);

/**
 * The ft_isalpha() function tests for any character for which isupper(3) or
 * islower(3) is true.
 */
int	ft_isalpha(int c)
{
	if (ft_isupper(c) || ft_islower(c))
		return (1);
	else
		return (0);
}

static int	ft_isupper(int c)
{
	if ('A' <= c && c <= 'Z')
		return (1);
	else
		return (0);
}

static int	ft_islower(int c)
{
	if ('a' <= c && c <= 'z')
		return (1);
	else
		return (0);
}
