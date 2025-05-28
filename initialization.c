/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 13:35:35 by khiidenh          #+#    #+#             */
/*   Updated: 2025/05/28 17:51:52 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	initialize_philosophers(t_table *table)
{
	int	i;

	i = 0;

	//NULL CHECK
	while (i < table->num_philos)
	{
		table->philos[i].id = i;
		table->philos[i].times_ate = 0;
		table->philos[i].last_meal_time = 0;
		table->philos[i].table = table;
		table->philos[i].left_fork = &table->forks[i];
		table->philos[i].right_fork = &table->forks[(i + 1) % table->num_philos];
		if (i == table->num_philos - 1)
		{
			table->philos[i].left_fork = &table->forks[(i + 1) % table->num_philos];
			table->philos[i].right_fork = &table->forks[i];
		}
		i++;
	}
}

//we need to track the mutex initializations...
void	initialize(int argc, char *argv[], t_table *table)
{
	int	i;

	i = 0;
	table->num_philos = ft_atoi(argv[1]);
	table->time_to_die = ft_atoi(argv[2]);
	table->time_to_eat = ft_atoi(argv[3]);
	table->time_to_sleep = ft_atoi(argv[4]);
	table->time_to_think = (table->time_to_die - (table->time_to_eat + table->time_to_sleep)) / 2;
	if (table->time_to_think < 0)
		table->time_to_think = 0;
	if (argc - 1 == 5)
		table->must_eat_count = ft_atoi(argv[5]);
	else
		table->must_eat_count = -1;
	table->start_time = get_time();
	table->finished = 0;
	table->philos = NULL;
	table->forks = NULL;
	table->philos = malloc(sizeof(t_philo) * table->num_philos);
	if (table->philos == NULL)
		{
			printf(MEM_FAIL);
			clean_up(table, MALLOC, table->num_philos);
			exit(1);
		}

	table->forks = malloc(sizeof(pthread_mutex_t) * table->num_philos);
	//NULL CHECK
	pthread_mutex_init(&table->print_lock, NULL);
	pthread_mutex_init(&table->meal_check_lock, NULL);
	pthread_mutex_init(&table->finished_lock, NULL);
	while (i < table->num_philos)
	{
		pthread_mutex_init(&table->forks[i], 0);
		//ERROR CHECK
		i++;
	}
//	clean_up(table);

	//ERROR CHECK
	initialize_philosophers(table);

}
