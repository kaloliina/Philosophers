#include "philosophers.h"

int	ft_atoi(const char *nptr)
{
	int		i;
	int		check;
	long	result;

	i = 0;
	check = 1;
	result = 0;
	while (nptr[i] == 32 || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	if (nptr[i] == 43 || nptr[i] == 45)
	{
		if (nptr[i] == 45)
			check = -1 * check;
		i++;
	}
	while (nptr[i] >= 48 && nptr[i] <= 57)
	{
		result = result * 10 + (nptr[i] - '0');
		i++;
		if (check == 1 && result < 0)
			return ((int)LONG_MAX);
		if (check == -1 && result < 0)
			return ((int)LONG_MIN);
	}
	return ((int)result * check);
}

long int get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}