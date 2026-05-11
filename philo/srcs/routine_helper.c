#include "philosopher.h"
#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>

int	check_deadbool(t_philos *philo)
{
	int	dead;

	pthread_mutex_lock(philo->death_mutex);
	dead = philo->data->deadbool;
	pthread_mutex_unlock(philo->death_mutex);
	return (dead);
}

int	done_eat_count(t_data *args)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&args->counter_mutex);
	if (args->finished_count == args->philo_num)
		i = args->philo_num;
	pthread_mutex_unlock(&args->counter_mutex);
	return (i);
}

long	get_time_ms(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	wait_start(t_data *args)
{
	while (1)
	{
		pthread_mutex_lock(&args->ready_mutex);
		if (args->ready == 1)
		{
			pthread_mutex_unlock(&args->ready_mutex);
			break ;
		}
		pthread_mutex_unlock(&args->ready_mutex);
		usleep(100);
	}
}

void	*print_dead(t_data *args, int i)
{
	pthread_mutex_lock(&args->print_mutex);
	pthread_mutex_lock(&args->death_mutex);
	args->deadbool = 1;
	printf("%ld %d died\n", (get_time_ms() - args->start_time),
		args->philo[i].id);
	pthread_mutex_unlock(&args->death_mutex);
	pthread_mutex_unlock(&args->print_mutex);
	return (NULL);
}
