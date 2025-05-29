/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors_and_exits.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 14:37:58 by khiidenh          #+#    #+#             */
/*   Updated: 2025/05/29 16:17:01 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	clean_ups(t_table *table, t_clean_up clean_up)
{
	int	i;

	if (clean_up.cause_of_failure == MALLOC)
		printf(MEM_FAIL);
	if (clean_up.cause_of_failure == MUTEX)
		printf(MUTEX_FAIL);
	if (clean_up.cause_of_failure == THREAD)
		printf(THREAD_FAIL);
	i = 0;
	while (clean_up.destroy_forks == true && i < clean_up.fork_amount)
	{
		pthread_mutex_destroy(&table->forks[i]);
		i++;
	}
	if (clean_up.destroy_print == true)
		pthread_mutex_destroy(&table->print_lock);
	if (clean_up.destroy_meal == true)
		pthread_mutex_destroy(&table->meal_check_lock);
	if (clean_up.destroy_finished == true)
		pthread_mutex_destroy(&table->finished_lock);
	if (table->forks != NULL)
		free (table->forks);
	if (table->philos != NULL)
		free (table->philos);
	free (table);
}

	//do we need a philosopher limit, or MAX INT limit???
	//also the numbers should be bigger than 0, except for the nbr of filos that are required to eat.
bool	is_valid_input(int argc, char *argv[])
{
	if (argc != 5 && argc != 6)
	{
		printf(INPUT_FORMAT);
		return (false);
	}
	if(check_is_digit(argv[1]) == false || check_is_digit(argv[2]) == false || check_is_digit(argv[3]) == false ||
	check_is_digit(argv[4]) == false)
	{
		printf(DIGIT_CHECK);
		return (false);
	}
	if (argc == 6 && check_is_digit(argv[5]) == false)
	{
		printf(DIGIT_CHECK);
		return (false);
	}
	return (true);
}
