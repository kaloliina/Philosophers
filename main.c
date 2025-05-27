#include "philosophers.h"

void	initialize_philosophers(t_table *table)
{
int	i;

i = 0;
table->philos = malloc(sizeof(t_philo) * table->num_philos);
//null check
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

void    initialize(int argc, char *argv[], t_table *table)
{
	int	i;

	i = 0;
	table->num_philos = ft_atoi(argv[1]);
	table->time_to_die = ft_atoi(argv[2]);
	table->time_to_eat = ft_atoi(argv[3]);
	table->time_to_sleep = ft_atoi(argv[4]);
	if (argc - 1 == 5)
		table->must_eat_count = ft_atoi(argv[5]);
	else
		table->must_eat_count = -1;
	table->forks = malloc(sizeof(pthread_mutex_t) * table->num_philos);
	//malloc check
	while (i < table->num_philos)
	{
		pthread_mutex_init(&table->forks[i], 0);
		//error check
		i++;
	}
	table->start_time = get_time();
	pthread_mutex_init(&table->print_lock, NULL);
	pthread_mutex_init(&table->meal_check_lock, NULL);
	initialize_philosophers(table);
	table->finished = 0;
}
void *philosopher_routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;

	while (philo->table->finished == 0)
	{
    // Try to pick up forks (lock mutexes)
    pthread_mutex_lock(philo->left_fork);
    pthread_mutex_lock(philo->right_fork);

    // Lock print mutex before printing to avoid jumbled output
    pthread_mutex_lock(&philo->table->print_lock);
    printf("Philosopher %d has taken forks %ld and %ld\n",
           philo->id,
           philo->left_fork - philo->table->forks,
           philo->right_fork - philo->table->forks);
    pthread_mutex_unlock(&philo->table->print_lock);
	pthread_mutex_lock(&philo->table->meal_check_lock);
	pthread_mutex_lock(&philo->table->print_lock);
	philo->last_meal_time = get_time() - philo->table->start_time;
	philo->times_ate++;
	printf("Philosopher %d ate at %ld\n", philo->id, philo->last_meal_time);
	pthread_mutex_unlock(&philo->table->print_lock);
	pthread_mutex_unlock(&philo->table->meal_check_lock);
    // Put down forks (unlock mutexes)
    pthread_mutex_unlock(philo->right_fork);
    pthread_mutex_unlock(philo->left_fork);
	usleep(philo->table->time_to_sleep);
	}
    return NULL;
}

//this needs to have check if the person even has input the must eat count
void *watcher_routine(void *arg)
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
				printf("DEAATTHHH\n");
//				return ;
			}
			if (table->must_eat_count >= table->philos[i].times_ate)
				meals_eaten = 1;
			else
				meals_eaten = -1;
			i++;
		}
		if (meals_eaten == 1)
			printf("EVERYONE HAS EATEN\n");
		i = 0;
	}
}

void start_philosophers(t_table *table)
{
    for (int i = 0; i < table->num_philos; i++)
    {
        pthread_create(&table->philos[i].thread_id, NULL, philosopher_routine, &table->philos[i]);
    }
	pthread_create(&table->watcher, NULL, watcher_routine, table);

    for (int i = 0; i < table->num_philos; i++)
    {
        pthread_join(table->philos[i].thread_id, NULL);
    }
}

int main(int argc, char *argv[])
{
t_table *table;
table = malloc(sizeof(t_table));
//null check
//input format check
initialize(argc, argv, table);
start_philosophers(table);

}