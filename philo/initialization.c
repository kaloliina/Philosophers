/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 13:35:35 by khiidenh          #+#    #+#             */
/*   Updated: 2025/06/04 14:08:52 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	initialize_philosophers(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->num_philos)
	{
		table->philos[i].id = i + 1;
		table->philos[i].times_ate = 0;
		table->philos[i].last_meal_time = table->start_time;
		table->philos[i].table = table;
		table->philos[i].left_fork = &table->forks[i];
		table->philos[i].right_fork = &table->forks[(i + 1)
			% table->num_philos];
		if (i == table->num_philos - 1 && table->num_philos != 1)
		{
			table->philos[i].left_fork = &table->forks[(i + 1)
				% table->num_philos];
			table->philos[i].right_fork = &table->forks[i];
		}
		i++;
	}
}

static t_clean_up	initialize_mutexes(t_table *table, t_clean_up clean_up)
{
	int	i;

	i = 0;
	(void)clean_up;
	if (pthread_mutex_init(&table->print_lock, NULL) != 0)
		return ((t_clean_up){1, MUTEX, 0, 0, 0});
	while (i < table->num_philos)
	{
		if (pthread_mutex_init(&table->forks[i], 0) != 0)
			return ((t_clean_up){1, MUTEX, 1, 1, i});
		i++;
	}
	return ((t_clean_up){0, SUCCESS, 1, 1, table->num_philos});
}

t_clean_up	initialize(int argc, char *argv[], t_table *table)
{
	t_clean_up	clean_up;

	clean_up = (t_clean_up){0, 0, 0, 0, 0};
	*table = (t_table){ft_atoi(argv[1]), ft_atoi(argv[2]), ft_atoi(argv[3]),
		ft_atoi(argv[4]), 0, -1, get_time(), 0, 0, {0}, 0};
	table->time_to_think = (table->time_to_die
			- (table->time_to_eat + table->time_to_sleep)) / 2;
	if (table->time_to_think < 1)
		table->time_to_think = 1;
	if (argc - 1 == 5)
		table->must_eat_count = ft_atoi(argv[5]);
	table->philos = malloc(sizeof(t_philo) * table->num_philos);
	if (table->philos == NULL)
		return ((t_clean_up){1, MALLOC, 0, 0, 0});
	table->forks = malloc(sizeof(pthread_mutex_t) * table->num_philos);
	if (table->forks == NULL)
		return ((t_clean_up){1, MALLOC, 0, 0, 0});
	clean_up = initialize_mutexes(table, clean_up);
	if (clean_up.early_failure == true)
		return (clean_up);
	initialize_philosophers(table);
	return ((t_clean_up){0, SUCCESS, 1, 1, table->num_philos});
}
