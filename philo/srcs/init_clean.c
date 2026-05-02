#include "../includes/philosopher.h"
#include <stdio.h>
#include <stdlib.h>

static int	start_thread(t_data *args);

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

int	start_sim(t_data *args)
{
	int				i;

	args->philo = malloc(sizeof(t_philos) * (args->philo_num));
	args->fork = malloc(sizeof(pthread_mutex_t) * (args->philo_num));
	args->start_time = get_time_ms();
	philo_init(args);
	if (start_thread(args) == 1)
		return (1);
	pthread_mutex_lock(&args->ready_mutex);
	args->ready = 1;
	pthread_mutex_unlock(&args->ready_mutex);
	i = 0;
	while (i < args->philo_num)
	{
		pthread_join(args->philo[i].thread_id, NULL);
		i++;
	}
	pthread_join(args->monitoring, NULL);
	cleanup(args);
	return (1);
}

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

	i = 0;
	pthread_mutex_init(&args->death_mutex, NULL);
	pthread_mutex_init(&args->meal_time_mutex, NULL);
	pthread_mutex_init(&args->counter_mutex, NULL);
	pthread_mutex_init(&args->print_mutex, NULL);
	pthread_mutex_init(&args->ready_mutex, NULL);
	while (i < args->philo_num)
	{
		args->philo[i].id = i + 1;
		args->philo[i].last_meal_time = args->start_time;
		args->philo[i].counter = 0;
		args->philo[i].data = args;
		args->philo[i].meal_time_mutex = &args->meal_time_mutex;
		args->philo[i].death_mutex = &args->death_mutex;
		pthread_mutex_init(&args->fork[i], NULL);
		args->philo[i].l_fork = &args->fork[i];
		args->philo[i].r_fork = &args->fork[(i + 1) % args->philo_num];
		i++;
	}
	args->deadbool = 0;
	args->finished_count = 0;
	args->ready = 0;
}

static int	start_thread(t_data *args)
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
		pthread_create(&args->philo[i].thread_id,
			NULL, routine, &args->philo[i]);
		i++;
	}
	pthread_create(&args->monitoring, NULL, (void *)monitoring_thread, args);
	return (0);
}
