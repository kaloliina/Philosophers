/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 14:38:04 by khiidenh          #+#    #+#             */
/*   Updated: 2025/06/04 14:17:13 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	set_finished(t_table *table, int id, char *message)
{
	print_message(table, message, id);
	table->finished = true;
}

static void	watcher_routine(t_table *table)
{
	int		i;
	int		all_ate;

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
}

static t_clean_up	start_philosophers(t_table *table, t_clean_up clean_up)
{
	int	i;
	int	threads;

	i = 0;
	while (i < table->num_philos)
	{
		if (pthread_create(&table->philos[i].thread_id, NULL,
				philosopher_routine, &table->philos[i]) != 0)
		{
			table->finished = 1;
			clean_up = (t_clean_up){1, THREAD, 1, 1, table->num_philos};
			break ;
		}
		i++;
	}
	watcher_routine(table);
	threads = i;
	i = 0;
	while (i < threads)
	{
		if (pthread_join(table->philos[i++].thread_id, NULL) != 0)
			write_error(THREAD_JOIN_FAIL);
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
		write_error(MEM_FAIL);
		return (1);
	}
	clean_up = initialize(argc, argv, table);
	if (clean_up.early_failure == false)
		clean_up = start_philosophers(table, clean_up);
	if (clean_up.early_failure == false)
	{
		clean_ups(table, clean_up);
		return (0);
	}
	else
	{
		clean_ups(table, clean_up);
		return (1);
	}
}
