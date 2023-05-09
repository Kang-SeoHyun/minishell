#include "libft.h"

/**
 * The ft_putendl_fd() function outputs the string ’s’ to the given file
 * descriptor followed by a newline.
 */
void	ft_putendl_fd(char *s, int fd)
{
	ft_putstr_fd(s, fd);
	ft_putchar_fd('\n', fd);
}
