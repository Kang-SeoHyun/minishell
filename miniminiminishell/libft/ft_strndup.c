#include "libft.h"

/**
 * The ft_strndup() function copies at most n characters from the string s1
 * always NUL terminating the copied string.
 */
char	*ft_strndup(const char *s1, size_t n)
{
	char	*str;

	str = malloc(sizeof(char) * (n + 1));
	if (str == NULL)
		return (NULL);
	ft_memcpy(str, s1, n);
	str[n] = '\0';
	return (str);
}
