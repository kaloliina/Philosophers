/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   watcher.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:32:52 by khiidenh          #+#    #+#             */
/*   Updated: 2025/06/02 16:54:20 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

//You could remove the all philosophers have eaten their fill part
//This would mean adjusting the print message function
static void	*set_finished(t_table *table, int id, char *message)
{
	table->finished = true;
	print_message(table, message, id);
	return (NULL);
}

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
			if (table->philos[i].times_ate < table->must_eat_count)
				all_ate = 0;
			i++;
		}
		if (table->must_eat_count != -1 && all_ate == 1)
			return (set_finished(table, -1, FULL));
		usleep(100);
	}
	return (NULL);
}
