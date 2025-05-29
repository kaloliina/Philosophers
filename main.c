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

void	sleepy_time(t_philo *philo, long int time, char *message)
{
	time_t total_time;

	total_time = time + get_time();
	if (philo->table->finished == 0)
		print_message(philo->table, message, philo->id);
	while (get_time() < total_time)
	{
		pthread_mutex_lock(&philo->table->finished_lock);
		if (philo->table->finished == 1)
		{
			pthread_mutex_unlock(&philo->table->finished_lock);
			break;
		}
		pthread_mutex_unlock(&philo->table->finished_lock);
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
		sleepy_time(philo, philo->table->time_to_think, THINK);
	while (philo->table->finished == 0)
	{
		pthread_mutex_lock(philo->left_fork);
		print_message(philo->table, FORK, philo->id);
		pthread_mutex_lock(philo->right_fork);
		print_message(philo->table, FORK, philo->id);
		philo->last_meal_time = get_time();
		sleepy_time(philo, philo->table->time_to_eat, EAT);
		if (philo->table->finished == 1)
			break;
		pthread_mutex_lock(&philo->table->meal_check_lock);
		philo->times_ate++;
		pthread_mutex_unlock(&philo->table->meal_check_lock);
		if (philo->table->finished == 1)
			break;
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
		sleepy_time(philo, philo->table->time_to_sleep, SLEEP);
		sleepy_time(philo, philo->table->time_to_think, THINK);
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
		while (i < table->num_philos && table->finished == 0)
		{
			if (get_time() - table->philos[i].last_meal_time > table->time_to_die)
			{
				pthread_mutex_lock(&table->finished_lock);
				table->finished = 1;
				pthread_mutex_unlock(&table->finished_lock);
				print_message(table, DIE, table->philos[i].id);
				return (NULL);
			}
			pthread_mutex_lock(&table->meal_check_lock);
			if (table->must_eat_count != -1 && table->philos[i].times_ate < table->must_eat_count)
				all_ate = 0;
			pthread_mutex_unlock(&table->meal_check_lock);
			i++;
		}
		if (table->must_eat_count != -1 && all_ate == 1 && table->finished == 0)
		{
			pthread_mutex_lock(&table->finished_lock);
			table->finished = 1;
			pthread_mutex_unlock(&table->finished_lock);
			print_message(table, FULL, -1);
			return (NULL);
		}
	}
	return (NULL);
}

//remember lone philo routine
t_clean_up	start_philosophers(t_table *table, t_clean_up clean_up)
{
	int	i;
	int	threads;

	i = 0;
	while (i < table->num_philos)
	{
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
