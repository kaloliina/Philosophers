/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:36:23 by khiidenh          #+#    #+#             */
/*   Updated: 2025/06/03 11:44:39 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	sleepy_time(t_philo *philo, long int time, char *message)
{
	time_t	total_time;

	total_time = time + get_time();
	if (philo->table->finished == false)
		print_message(philo->table, message, philo->id);
	else
		return ;
	while (get_time() < total_time)
	{
		if (philo->table->finished == true)
			break ;
		usleep(1000);
	}
}

static void	release_forks(t_philo *philo, int fork_amount)
{
	pthread_mutex_unlock(philo->left_fork);
	if (philo->table->num_philos == 1 || fork_amount == 1)
		return ;
	pthread_mutex_unlock(philo->right_fork);
}

static int	handle_forks(t_philo *philo)
{
	int	forks_picked;

	forks_picked = 0;
	if (philo->table->finished == false)
	{
		pthread_mutex_lock(philo->left_fork);
		print_message(philo->table, FORK, philo->id);
		forks_picked++;
	}
	if (philo->table->num_philos == 1)
	{
		while (philo->table->finished == false)
			usleep(1000);
		return (forks_picked);
	}
	if (philo->table->finished == false)
	{
		pthread_mutex_lock(philo->right_fork);
		print_message(philo->table, FORK, philo->id);
		forks_picked++;
	}
	return (forks_picked);
}

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;
	int		fork_amount;

	philo = (t_philo *)arg;
	if (philo->table->must_eat_count == 0)
		return (NULL);
	if (philo->id % 2 == 0)
		sleepy_time(philo, philo->table->time_to_think, THINK);
	while (philo->table->finished == false)
	{
		fork_amount = handle_forks(philo);
		philo->last_meal_time = get_time();
		sleepy_time(philo, philo->table->time_to_eat, EAT);
		if (philo->table->finished == true)
		{
			release_forks(philo, fork_amount);
			break ;
		}
		if (philo->table->finished == false)
			philo->times_ate++;
		release_forks(philo, fork_amount);
		sleepy_time(philo, philo->table->time_to_sleep, SLEEP);
		sleepy_time(philo, philo->table->time_to_think, THINK);
	}
	return (NULL);
}
