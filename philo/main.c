#include "philosopher.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>

void	philo_init(t_data *args)
{
	int	i;

	i = 0;
	pthread_mutex_init(&args->death_mutex, NULL);
	pthread_mutex_init(&args->print_status, NULL);
	while (i < args->philo_num)
	{
		args->philo[i].id = i + 1;
		args->philo[i].meals_eaten = 0;
		args->philo[i].last_meal_time = args->start_time;
		args->philo[i].data = args;
		args->philo[i].print_status = &args->print_status;
		args->philo[i].death_mutex = &args->death_mutex;
		pthread_mutex_init(&args->fork[i], NULL);
		args->philo[i].l_fork = &args->fork[i];
		args->philo[i].r_fork = &args->fork[(i + 1) % args->philo_num];
		i++;
	}
}

void	print_status(t_philos *philo, char *s)
{
	long	now;

	pthread_mutex_lock(philo->print_status);
	now = get_time_ms();
	philo->current_time = now - (philo->data->start_time);
	printf("%ld Philo %d %s", philo->current_time, philo->id, s);
	pthread_mutex_unlock(philo->print_status);
}

void	*monitoring_thread(t_data *args)
{
	long	current;
	long	compare;
	long	result;
	int		i;

	i = 0;
	printf("monitoring thread init\n");
	while (1)
	{
		pthread_mutex_lock(args->philo[i].death_mutex);
		current = get_time_ms();
		compare = args->philo[i].last_meal_time;
		result = current - compare;
		if (result > (long)args->philo[i].data->t_die)
		{
			printf("last eat time = %ld\n", result);
			print_status(&args->philo[i], "\x1B[31mis dead\n\x1B[0m");
			args->deadbool = 1;
			pthread_mutex_unlock(args->philo[i].death_mutex);
			return (NULL);
		}
		pthread_mutex_unlock(args->philo[i].death_mutex);
		i++;
		usleep (100);
	}
}

void	philo_eat(t_philos *philo)
{
	pthread_mutex_lock(philo->l_fork);
	pthread_mutex_lock(philo->r_fork);
	print_status(philo, "has taken a fork\n\n");
	print_status(philo, "has taken a fork\n\n");
	print_status(philo, "is eating\n\n");
	pthread_mutex_lock(philo->death_mutex);
	philo->last_meal_time = get_time_ms();
	pthread_mutex_unlock(philo->death_mutex);
	usleep(philo->data->t_eat * 1000);
	pthread_mutex_unlock(philo->l_fork);
	pthread_mutex_unlock(philo->r_fork);
}

void	philo_sleep(t_philos *philo)
{
	print_status(philo, "is sleeping\n\n");
	usleep(philo->data->t_sleep * 1000);
}

void	philo_think(t_philos *philo)
{
	print_status(philo, "is thinking\n\n");
}

void	*routine(t_philos *philo)
{
	int	counter;

	printf("Hi,I am philo %d\n", philo->id);
	if ((philo->id % 2) == 0)
		usleep(1000);
	counter = 0;
	while (philo->data->eat_num == -1 || counter < philo->data->eat_num)
	{
		pthread_mutex_lock(philo->death_mutex);
		if (philo->data->deadbool == 1)
		{
			pthread_mutex_unlock(philo->death_mutex);
			break ;
		}
		pthread_mutex_unlock(philo->death_mutex);
		philo_eat(philo);
		philo_sleep(philo);
		philo_think(philo);
		counter++;
	}
	if (philo->data->deadbool)
	{
	}
	return (NULL);
}

long	get_time_ms(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

int	start_sim(t_data *args)
{
	int				i;

	printf("\n---start sim---\n");
	args->philo = malloc(sizeof(t_philos) * (args->philo_num));
	args->fork = malloc(sizeof(pthread_mutex_t) * (args->philo_num));
	args->start_time = get_time_ms();
	philo_init(args);
	printf("done init\n");
	i = 0;
	while (i < args->philo_num)
	{
		printf("creating threads\n");
		if (args->philo_num == 1)
			return (0);
		pthread_create(&args->philo[i].thread_id, NULL, (void *)routine, &args->philo[i]);
		i++;
	}
	pthread_create(&args->monitoring, NULL, (void *)monitoring_thread, args);
	i = 0;
	while (i < args->philo_num)
	{
		pthread_join(args->philo[i].thread_id, NULL);
		i++;
	}
	pthread_join(args->monitoring, NULL);
	printf("\n---stopping simulator---\n");
	printf("end\n");
	return (1);
}

int	main(int ac, char **av)
{
	t_data	*args;

	args = malloc(sizeof(t_data));
	if (!parsing_check(ac, av, args))
	{
		perror("parsing error");
		return (1);
	}
	if (!(start_sim(args)))
	{
		perror("start simulation error");
		return (1);
	}
	printf("end\n");
	return (0);
}
