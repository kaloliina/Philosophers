#include "philosophers.h"

static bool	is_finished(t_table *table)
{
	pthread_mutex_lock(&table->finished_lock);
	if (table->finished == 1)
	{
		pthread_mutex_unlock(&table->finished_lock);
		return (true);
	}
	pthread_mutex_unlock(&table->finished_lock);
	return (false);
}

static void	sleepy_time(t_philo *philo, long int time, char *message)
{
	time_t total_time;

	total_time = time + get_time();
	if (is_finished(philo->table) == false)
		print_message(philo->table, message, philo->id);
	while (get_time() < total_time)
	{
		if (is_finished(philo->table) == true)
			break;
		usleep(1000);
	}
}

static void release_forks(t_philo *philo, int fork_amount)
{
    pthread_mutex_unlock(philo->left_fork);
    if (philo->table->num_philos == 1 || fork_amount == 1)
        return ;
    pthread_mutex_unlock(philo->right_fork);
}

static void	handle_forks(t_philo *philo)
{
    if (is_finished(philo->table) == true)
        return ;
	pthread_mutex_lock(philo->left_fork);
    if (is_finished(philo->table) == true)
        return (release_forks(philo, 1));
    else
		print_message(philo->table, FORK, philo->id);
    if (philo->table->num_philos == 1)
    {
        while (is_finished(philo->table) == false)
            usleep(1000);
        return ;
    }
    pthread_mutex_lock(philo->right_fork);
    if (is_finished(philo->table) == true)
        return (release_forks(philo, 2));
    else
		print_message(philo->table, FORK, philo->id);
}

void	*philosopher_routine(void *arg)
{
	t_philo	*philo = (t_philo *)arg;

    if (philo->table->must_eat_count == 0)
        return (NULL);
	if (philo->id % 2 == 0)
		sleepy_time(philo, philo->table->time_to_think, THINK);
	while (is_finished(philo->table) == false)
	{
		handle_forks(philo);
		philo->last_meal_time = get_time();
		sleepy_time(philo, philo->table->time_to_eat, EAT);
		if (is_finished(philo->table) == true)
		{
			release_forks(philo, 2);
			break ;
		}
		pthread_mutex_lock(&philo->table->meal_check_lock);
        if (is_finished(philo->table) == false)
		    philo->times_ate++;      
		pthread_mutex_unlock(&philo->table->meal_check_lock);
        release_forks(philo, 2);
        if (is_finished(philo->table) == true)
            break;
        if (is_finished(philo->table) == false)
		    sleepy_time(philo, philo->table->time_to_sleep, SLEEP);
        if (is_finished(philo->table) == false)
	    	sleepy_time(philo, philo->table->time_to_think, THINK);
	}
	return NULL;
}