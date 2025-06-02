/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   watcher.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:32:52 by khiidenh          #+#    #+#             */
/*   Updated: 2025/06/02 13:34:46 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	*set_finished(t_table *table, int id, char *message)
{
	pthread_mutex_lock(&table->finished_lock);
	table->finished = 1;
	pthread_mutex_unlock(&table->finished_lock);
	print_message(table, message, id);
	return (NULL);
}

//you dont need to say that philosophers have eaten their fill
void	*watcher_routine(void *arg)
{
	t_table	*table;
	int		i;
	int		all_ate;

	table = (t_table *)arg;
	if (table->must_eat_count == 0)
		return (set_finished(table, -1, FULL));
	while (true)
	{
		i = 0;
		all_ate = 1;
		while (i < table->num_philos)
		{
			if (get_time() - table->philos[i].last_meal_time
				> table->time_to_die)
				return (set_finished(table, table->philos[i].id, DIE));
			pthread_mutex_lock(&table->meal_check_lock);
			if (table->must_eat_count != -1 && table->philos[i].times_ate
				< table->must_eat_count)
				all_ate = 0;
			pthread_mutex_unlock(&table->meal_check_lock);
			i++;
		}
		if (table->must_eat_count != -1 && all_ate == 1)
			return (set_finished(table, -1, FULL));
		usleep(100);
	}
	return (NULL);
}
