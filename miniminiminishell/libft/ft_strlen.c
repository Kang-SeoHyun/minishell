#include "libft.h"

/**
 * The ft_strlen() function computes the length of the string s.
 */
size_t	ft_strlen(const char *s)
{
	size_t	len;

	len = 0;
	if (s)
		while (s[len])
			len++;
	return (len);
}
