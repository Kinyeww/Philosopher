#include "philosopher.h"
#include <stdio.h>
#include <stdlib.h>

void	one_philo(t_philos *philo)
{
	pthread_mutex_lock(philo->l_fork);
	print_status(philo, "has taken a fork");
	ft_usleep(philo->data->t_die, philo);
	printf("%ld 1 died\n", get_time_ms() - philo->data->start_time);
	pthread_mutex_unlock(philo->l_fork);
}

void	philo_init(t_data *args)
{
	int	i;

	i = -1;
	pthread_mutex_init(&args->death_mutex, NULL);
	pthread_mutex_init(&args->meal_time_mutex, NULL);
	pthread_mutex_init(&args->counter_mutex, NULL);
	pthread_mutex_init(&args->print_mutex, NULL);
	pthread_mutex_init(&args->ready_mutex, NULL);
	while (++i < args->philo_num)
	{
		args->philo[i].id = i + 1;
		args->philo[i].counter = 0;
		args->philo[i].data = args;
		args->philo[i].finished = 0;
		args->philo[i].last_meal_time = get_time_ms();
		args->philo[i].meal_time_mutex = &args->meal_time_mutex;
		args->philo[i].death_mutex = &args->death_mutex;
		pthread_mutex_init(&args->fork[i], NULL);
		args->philo[i].l_fork = &args->fork[i];
		args->philo[i].r_fork = &args->fork[(i + 1) % args->philo_num];
	}
	args->deadbool = 0;
	args->finished_count = 0;
	args->ready = 0;
	args->start_time = get_time_ms();
}

int	start_thread(t_data *args)
{
	int	i;

	if (args->philo_num == 1)
	{
		one_philo(args->philo);
		return (1);
	}
	i = 0;
	while (i < args->philo_num)
	{
		if (pthread_create(&args->philo[i].thread_id,
				NULL, routine, &args->philo[i]) != 0)
			return (1);
		i++;
	}
	if (pthread_create(&args->monitoring, NULL,
			(void *)monitoring_thread, args))
		return (1);
	return (0);
}

void	cleanup(t_data *args)
{
	int	i;

	i = 0;
	while (i < args->philo_num)
	{
		pthread_mutex_destroy(&args->fork[i]);
		i++;
	}
	pthread_mutex_destroy(&args->death_mutex);
	pthread_mutex_destroy(&args->meal_time_mutex);
	pthread_mutex_destroy(&args->counter_mutex);
	pthread_mutex_destroy(&args->print_mutex);
	pthread_mutex_destroy(&args->ready_mutex);
	free(args->fork);
	free(args->philo);
	free(args);
}

void	set_last_meal_time(t_data *args)
{
	int	i;

	i = 0;
	while (i < args->philo_num)
	{
		args->philo[i].last_meal_time = args->start_time;
		i++;
	}
}
