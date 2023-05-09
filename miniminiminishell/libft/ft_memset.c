#include "libft.h"

/**
 * The ft_memset() function writes len bytes of value c (converted to an
 * unsigned char) to the string b.
 */
void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*b_ptr;

	b_ptr = (unsigned char *)b;
	while (len--)
		*b_ptr++ = c;
	return (b);
}
