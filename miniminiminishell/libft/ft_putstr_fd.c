#include "libft.h"

/**
 * The ft_putstr_fd() function outputs the string ’s’ to the given file
 * descriptor.
 */
void	ft_putstr_fd(char *s, int fd)
{
	write(fd, s, ft_strlen(s));
}
