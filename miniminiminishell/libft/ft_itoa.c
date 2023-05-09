#include "libft.h"

static int	get_num_of_digit(long long *n, int *minus)
{
	int			i;
	long long	nbr;

	i = 0;
	*minus = 0;
	if (*n < 0)
	{
		*n *= -1;
		*minus = 1;
	}
	nbr = *n;
	if (nbr == 0)
		return (1);
	while (nbr > 0)
	{
		nbr /= 10;
		i++;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	int			n_digit;
	long long	l_n;
	char		*res;
	int			minus;
	int			i;

	l_n = (long long)n;
	n_digit = get_num_of_digit(&l_n, &minus);
	res = (char *)malloc(sizeof(char) * (minus + n_digit + 1));
	if (!res)
		return (0);
	i = minus + n_digit;
	res[i] = '\0';
	if (minus)
		res[0] = '-';
	while (--i >= minus)
	{
		res[i] = l_n % 10 + '0';
		l_n /= 10;
	}
	return (res);
}
