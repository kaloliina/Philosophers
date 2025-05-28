/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 14:38:04 by khiidenh          #+#    #+#             */
/*   Updated: 2025/05/28 17:44:43 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	sleepy_time(t_table *table, long int time)
{
	time_t total_time;

	total_time = time + get_time();
	while (get_time() < total_time)
	{
		pthread_mutex_lock(&table->finished_lock);
		if (table->finished == 1)
		{
			pthread_mutex_unlock(&table->finished_lock);
			break;
		}
		pthread_mutex_unlock(&table->finished_lock);
		usleep(10);
	}
}

//maybe we need a lock for simulation to be finished as well???
void	*philosopher_routine(void *arg)
{
	t_philo	*philo = (t_philo *)arg;

//do I have to lock this as well? It feels like not because we are only reading start time and we are not modifying the start time.
//And each philosopher is its own...??
	philo->last_meal_time = philo->table->start_time;
	if (philo->id % 2 == 0)
	{
		print_message(philo->table, THINK, philo->id);
		sleepy_time(philo->table, philo->table->time_to_think);
	}
	while (philo->table->finished == 0)
	{
		pthread_mutex_lock(philo->left_fork);
		print_message(philo->table, FORK, philo->id);
		pthread_mutex_lock(philo->right_fork);
		print_message(philo->table, FORK, philo->id);
		philo->last_meal_time = get_time();
		print_message(philo->table, EAT, philo->id);
		sleepy_time(philo->table, philo->table->time_to_eat);
		if (philo->table->finished == 1)
			break;
		pthread_mutex_lock(&philo->table->meal_check_lock);
		philo->times_ate++;
		pthread_mutex_unlock(&philo->table->meal_check_lock);
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
		print_message(philo->table, SLEEP, philo->id);
		sleepy_time(philo->table, philo->table->time_to_sleep);
		print_message(philo->table, THINK, philo->id);
		sleepy_time(philo->table, philo->table->time_to_think);
	}
	return NULL;
}

//there is something weird going on with this one, the philosophers are eating much more than they are meant to :D
//when times_ate gets increased, we should immediattely be able to recognize this...
void	*watcher_routine(void *arg)
{
	t_table	*table = (t_table *)arg;
	int	i;
	int	all_ate;

	while (true)
	{
		i = 0;
		all_ate = 1;
		while (i < table->num_philos)
		{
			if (get_time() - table->philos[i].last_meal_time > table->time_to_die)
			{
				pthread_mutex_lock(&table->finished_lock);
				table->finished = 1;
				pthread_mutex_unlock(&table->finished_lock);
				print_message(table, DIE, table->philos[i].id);
				exit(0);
			}
			pthread_mutex_lock(&table->meal_check_lock);
			if (table->must_eat_count != -1 && table->philos[i].times_ate < table->must_eat_count)
				all_ate = 0;
			pthread_mutex_unlock(&table->meal_check_lock);
			i++;
		}
		if (table->must_eat_count != -1 && all_ate == 1)
		{
			pthread_mutex_lock(&table->finished_lock);
			table->finished = 1;
			pthread_mutex_unlock(&table->finished_lock);
			print_message(table, FULL, -1);
			exit(0);
		}
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

	if (argc != 5 && argc != 6)
	{
		printf(INPUT_FORMAT);
		return (1);
	}
	//do we need a philosopher limit, or MAX INT limit???
	if(check_is_digit(argv[1]) == false || check_is_digit(argv[2]) == false || check_is_digit(argv[3]) == false ||
	check_is_digit(argv[4]) == false)
	{
		printf(DIGIT_CHECK);
		return (1);
	}
	if (argc == 6 && check_is_digit(argv[5]) == false)
	{
		printf(DIGIT_CHECK);
		return (1);
	}
	table = malloc(sizeof(t_table));
	if (table == NULL)
	{
		printf(MEM_FAIL);
		return (1);
	}
	initialize(argc, argv, table);
	start_philosophers(table);
	clean_up(table, SUCCESS, table->num_philos);
}
