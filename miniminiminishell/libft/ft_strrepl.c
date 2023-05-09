#include "libft.h"

char	*ft_strrepl(char *s, int c1, int c2)
{
	char	*s_ptr;

	s_ptr = s;
	while (*s_ptr)
	{
		if (*s_ptr == (char)c1)
			*s_ptr = (char)c2;
		s_ptr++;
	}
	return (s);
}
