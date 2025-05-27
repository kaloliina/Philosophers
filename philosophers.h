#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
#include <stdlib.h>
#include <limits.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
typedef struct s_table {
    int num_philos;
    long int    time_to_die;
    long int    time_to_eat;
    long int    time_to_sleep;
    int         must_eat_count;
    long int    start_time;
    pthread_t   watcher;
    pthread_mutex_t *forks;
    pthread_mutex_t print_lock;
    pthread_mutex_t meal_check_lock;
    struct s_philo *philos;
    int         finished;
}   t_table;

typedef struct s_philo {
    int id;
    int times_ate;
    long int last_meal_time;
    pthread_t thread_id;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    t_table *table;
} t_philo;

int	ft_atoi(const char *nptr);
long int get_time(void);

#endif