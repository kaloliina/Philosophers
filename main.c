/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 14:38:04 by khiidenh          #+#    #+#             */
/*   Updated: 2025/05/27 15:23:49 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	sleepy_time(t_table *table, long int time)
{
	time_t total_time;

	total_time = time + get_time();
	while (get_time() < total_time)
	{
		if (table->finished == 1)
			break;
		usleep(10);
	}
}

void	*philosopher_routine(void *arg)
{
	t_philo	*philo = (t_philo *)arg;

	philo->last_meal_time = philo->table->start_time;
	while (philo->table->finished == 0)
	{
		// Try to pick up forks (lock mutexes)
		pthread_mutex_lock(philo->left_fork);
		print_message(philo->table, FORK, philo->id);
		pthread_mutex_lock(philo->right_fork);
		print_message(philo->table, FORK, philo->id);

		philo->last_meal_time = get_time();
		print_message(philo->table, EAT, philo->id);
		sleepy_time(philo->table, philo->table->time_to_eat);

		pthread_mutex_lock(&philo->table->meal_check_lock);
		philo->times_ate++;
		pthread_mutex_unlock(&philo->table->meal_check_lock);

		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
		print_message(philo->table, SLEEP, philo->id);
		sleepy_time(philo->table, philo->table->time_to_sleep);
	}
	return NULL;
}

//there is something weird going on with this one, the philosophers are eating much more than they are meant to :D
//when times_ate gets increased, we should immediattely be able to recognize this...
void	*watcher_routine(void *arg)
{
	t_table *table = (t_table *)arg;
	int i;
	int meals_eaten;

	meals_eaten = -1;
	i = 0;
	while (true)
	{
		while (i < table->num_philos)
		{
			if (get_time() - table->philos[i].last_meal_time > table->time_to_die)
			{
				table->finished = 1;
				print_message(table, DIE, table->philos[i].id);
				exit(1);
			}
			if (table->must_eat_count != -1)
			{
			if (table->must_eat_count <= table->philos[i].times_ate)
				meals_eaten = 1;
			else
				meals_eaten = -1;
			}
			i++;
		}
		if (table->must_eat_count != -1 && meals_eaten == 1)
		{
			table->finished = 1;
			print_message(table, FULL, -1);
			exit(1);
		}
		i = 0;
	}
}

void	start_philosophers(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->num_philos)
	{
		pthread_create(&table->philos[i].thread_id, NULL, philosopher_routine, &table->philos[i]);
		i++;
	}
	i = 0;
	pthread_create(&table->watcher, NULL, watcher_routine, table);
	while (i < table->num_philos)
	{
		pthread_join(table->philos[i].thread_id, NULL);
		i++;
	}
	pthread_join(table->watcher, NULL);
}

int	main(int argc, char *argv[])
{
	t_table	*table;

	table = malloc(sizeof(t_table));
	//NULL CHECK
	//INPUT FORMAT CHECK
	initialize(argc, argv, table);
	start_philosophers(table);
}
