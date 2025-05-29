/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 14:38:14 by khiidenh          #+#    #+#             */
/*   Updated: 2025/05/29 16:17:47 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

//check this one
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

void	print_message(t_table *table, char *message, int identifier)
{
	pthread_mutex_lock(&table->print_lock);
	if (identifier == -1)
		printf("%s", message);
	else
		printf(message, get_time(), identifier);
	pthread_mutex_unlock(&table->print_lock);
}

bool	check_is_digit(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '\0')
		return (false);
	while (str[i] != '\0')
	{
		if (!(str[i] >= 48 && str[i] <= 57))
			return (false);
		i++;
	}
	return (true);
}
