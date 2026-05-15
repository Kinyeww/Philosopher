#include "philosopher.h"
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int	main(int ac, char **av)
{
	t_data	*args;

	args = malloc(sizeof(t_data));
	if (!args)
		return (1);
	if (!parsing_check(ac, av, args))
	{
		free(args);
		printf("argument error\n");
		return (1);
	}
	if (!(start_sim(args)))
	{
		printf("start simulation error\n");
		return (1);
	}
	return (0);
}

int	start_sim(t_data *args)
{
	int				i;

	args->philo = malloc(sizeof(t_philos) * (args->philo_num));
	args->fork = malloc(sizeof(pthread_mutex_t) * (args->philo_num));
	if (!args->philo || !args->fork)
		return (0);
	philo_init(args);
	if (start_thread(args) == 1)
	{
		cleanup(args);
		return (1);
	}
	pthread_mutex_lock(&args->ready_mutex);
	args->start_time = get_time_ms();
	set_last_meal_time(args);
	args->ready = 1;
	pthread_mutex_unlock(&args->ready_mutex);
	i = -1;
	while (++i < args->philo_num)
		pthread_join(args->philo[i].thread_id, NULL);
	pthread_join(args->monitoring, NULL);
	cleanup(args);
	return (1);
}

long	get_time_ms(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}
