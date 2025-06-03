/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors_and_exits.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 14:37:58 by khiidenh          #+#    #+#             */
/*   Updated: 2025/06/03 11:11:40 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	clean_ups(t_table *table, t_clean_up clean_up)
{
	int	i;

	if (clean_up.cause_of_failure == MALLOC)
		write_error(MEM_FAIL);
	if (clean_up.cause_of_failure == MUTEX)
		write_error(MUTEX_FAIL);
	if (clean_up.cause_of_failure == THREAD)
		write_error(THREAD_FAIL);
	i = 0;
	while (clean_up.destroy_forks == true && i < clean_up.fork_amount)
	{
		if (pthread_mutex_destroy(&table->forks[i]) != 0)
			write_error(MUTEX_DESTROY_FAIL);
		i++;
	}
	if (clean_up.destroy_print == true)
	{
		if (pthread_mutex_destroy(&table->print_lock) != 0)
			write_error(MUTEX_DESTROY_FAIL);
	}
	if (table->forks != NULL)
		free (table->forks);
	if (table->philos != NULL)
		free (table->philos);
	free (table);
}

bool	is_valid_input(int argc, char *argv[])
{
	if (argc != 5 && argc != 6)
	{
		write_error(INPUT_FORMAT);
		return (false);
	}
	if (check_is_digit(argv[1]) == false || check_is_digit(argv[2]) == false
		|| check_is_digit(argv[3]) == false || check_is_digit(argv[4]) == false
		|| ft_atoi(argv[1]) == -1 || ft_atoi(argv[2]) == -1
		|| ft_atoi(argv[3]) == -1 || ft_atoi(argv[4]) == -1)
	{
		write_error(DIGIT_CHECK);
		return (false);
	}
	if (argv[1][0] == '0')
	{
		write_error(NUM_OF_PHILOS);
		return (false);
	}
	if ((argc == 6 && check_is_digit(argv[5]) == false)
		|| (argc == 6 && ft_atoi(argv[5]) == -1))
	{
		write_error(DIGIT_CHECK);
		return (false);
	}
	return (true);
}
