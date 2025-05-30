/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 14:38:04 by khiidenh          #+#    #+#             */
/*   Updated: 2025/05/29 16:17:16 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

t_clean_up	start_philosophers(t_table *table, t_clean_up clean_up)
{
	int	i;
	int	threads;

	i = 0;
	while (i < table->num_philos)
	{
		table->start_time = get_time();
		table->philos[i].last_meal_time = table->start_time;
		if (pthread_create(&table->philos[i].thread_id, NULL, philosopher_routine, &table->philos[i]) != 0)
		{
			clean_up = (t_clean_up){1, THREAD, 1, 1, 1, 1, table->num_philos};
			break ;
		}
		i++;
	}
	threads = i;
	i = 0;
	if (pthread_create(&table->watcher, NULL, watcher_routine, table) != 0)
		clean_up = (t_clean_up){1, THREAD, 1, 1, 1, 1, table->num_philos};
	else
		pthread_join(table->watcher, NULL);
	while (i < threads)
	{
		pthread_join(table->philos[i].thread_id, NULL);
		i++;
	}
	return (clean_up);
}

int	main(int argc, char *argv[])
{
	t_table		*table;
	t_clean_up	clean_up;

	if (is_valid_input(argc, argv) == false)
		return (1);
	table = malloc(sizeof(t_table));
	if (table == NULL)
	{
		printf(MEM_FAIL);
		return (1);
	}
	clean_up = initialize(argc, argv, table);
	if (clean_up.early_failure == false)
		clean_up = start_philosophers(table, clean_up);
	clean_ups(table, clean_up);
	return (0);
}
