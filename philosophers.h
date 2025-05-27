/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 14:38:10 by khiidenh          #+#    #+#             */
/*   Updated: 2025/05/27 14:38:20 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
#include <stdlib.h>
#include <limits.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#define MEM_FAIL "Failed to allocate memory.\n"
#define THREAD_FAIL "Failed to create a thread.\n"
#define MUTEX_FAIL "Failed to create a mutex.\n"
#define FORK "%d %d has taken a fork\n"
#define EAT "%d %d is eating\n"
#define SLEEP "%d %d is sleeping\n"
#define THINK "%d %d is thinking\n"
#define DIE "%d %d died\n"
#define FULL "All philosophers have eaten their fill!\n"

typedef struct s_table {
	int				num_philos;
	long int		time_to_die;
	long int		time_to_eat;
	long int		time_to_sleep;
	int				must_eat_count;
	long int		start_time;
	pthread_t		watcher;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	meal_check_lock;
	struct s_philo	*philos;
	int				finished;
}	t_table;

typedef struct s_philo {
	int				id;
	int				times_ate;
	long int		last_meal_time;
	pthread_t		thread_id;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_table			*table;
}	t_philo;

//Initialization
void		initialize(int argc, char *argv[], t_table *table);

//Utils
int			ft_atoi(const char *nptr);
long int	get_time(void);
void		print_message(t_table *table, char *message, int identifier);

#endif
