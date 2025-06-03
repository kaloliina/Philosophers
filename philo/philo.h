/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 14:38:10 by khiidenh          #+#    #+#             */
/*   Updated: 2025/06/03 12:04:08 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdio.h>
# include <unistd.h>
# include <stdbool.h>
# include <stdatomic.h>
# define MEM_FAIL "Failed to allocate memory.\n"
# define THREAD_FAIL "Failed to create a thread.\n"
# define MUTEX_FAIL "Failed to create a mutex.\n"
# define MUTEX_DESTROY_FAIL "Failed to destroy a mutex.\n"
# define NUM_OF_PHILOS "There must be at least one philosopher sitting at the \
table\n"
# define FORK "%d %d has taken a fork\n"
# define EAT "%d %d is eating\n"
# define SLEEP "%d %d is sleeping\n"
# define THINK "%d %d is thinking\n"
# define DIE "%d %d died\n"
# define FULL "All philosophers have eaten their fill!\n"
# define INPUT_FORMAT "Expected format: <number_of_philosophers> \
<time_to_die> <time_to_eat> <time_to_sleep> \
[number_of_times_each_philosopher_must_eat]\n"
# define DIGIT_CHECK "Input should contain only of digits of positive \
value and not exceed maximum integer value.\n"

enum e_fail_stages
{
	SUCCESS,
	MALLOC,
	MUTEX,
	THREAD,
};

typedef struct s_table
{
	int				num_philos;
	long int		time_to_die;
	long int		time_to_eat;
	long int		time_to_sleep;
	long int		time_to_think;
	int				must_eat_count;
	long int		start_time;
	atomic_bool		finished;
	struct s_philo	*philos;
	pthread_t		watcher;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	*forks;
}	t_table;

typedef struct s_philo
{
	int				id;
	atomic_int		times_ate;
	atomic_long		last_meal_time;
	pthread_t		thread_id;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_table			*table;
}	t_philo;

typedef struct s_clean_up
{
	bool				early_failure;
	enum e_fail_stages	cause_of_failure;
	bool				destroy_print;
	bool				destroy_forks;
	int					fork_amount;
}	t_clean_up;

//Validation
bool		is_valid_input(int argc, char *argv[]);

//Initialization
t_clean_up	initialize(int argc, char *argv[], t_table *table);

//Routines
void		*watcher_routine(void *arg);
void		*philosopher_routine(void *arg);

//Utils
int			ft_atoi(const char *nptr);
bool		check_is_digit(char *str);
long int	get_time(void);
void		print_message(t_table *table, char *message, int identifier);
void		write_error(char *message);

//Clean up
void		clean_ups(t_table *table, t_clean_up clean_up);

#endif
