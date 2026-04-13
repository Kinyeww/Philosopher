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
	pthread_mutex_init(&args->meal_time_mutex, NULL);
	pthread_mutex_init(&args->counter_mutex, NULL);
	pthread_mutex_init(&args->print_mutex, NULL);
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
}

void	one_philo(t_philos *philo)
{
	pthread_mutex_lock(philo->l_fork);
	print_status(philo, "has taken a fork");
	ft_usleep(philo->data->t_die);
	print_status(philo, "died");
	pthread_mutex_unlock(philo->l_fork);
}

void	print_status(t_philos *philo, char *s)
{
	long	now;

	pthread_mutex_lock(&philo->data->print_mutex);
	pthread_mutex_lock(philo->death_mutex);
	if (philo->data->deadbool == 1)
	{
		pthread_mutex_unlock(&philo->data->print_mutex);
		pthread_mutex_unlock(philo->death_mutex);
		return ;
	}
	pthread_mutex_unlock(philo->death_mutex);
	now = get_time_ms();
	philo->current_time = now - (philo->data->start_time);
	printf("%ld %d %s\n\n", philo->current_time, philo->id, s);
	pthread_mutex_unlock(&philo->data->print_mutex);
}

int	done_eat_count(t_data *args)
{
	int	i;
	int	result;

	i = 0;
	result = 0;
	while (i < args->philo_num)
	{
		pthread_mutex_lock(&args->counter_mutex);
		if (args->philo[i].counter >= args->eat_num)
			result++;
		pthread_mutex_unlock(&args->counter_mutex);
		i++;
	}
	return (result);
}

void	*monitoring_thread(t_data *args)
{
	long	current;
	long	compare;
	long	result;
	int		i;

	i = 0;
	while (1)
	{
		if (i == args->philo_num)
			i = 0;
		if (args->eat_num != -1 && done_eat_count(args) == args->philo_num)
			return (NULL);
		pthread_mutex_lock(&args->death_mutex);
		current = get_time_ms();
		pthread_mutex_lock(&args->meal_time_mutex);
		compare = args->philo[i].last_meal_time;
		result = current - compare;
		pthread_mutex_unlock(&args->meal_time_mutex);
		if (result > (long)args->philo[i].data->t_die)
		{
			pthread_mutex_lock(&args->print_mutex);
			printf("last eat time = %ldms, need within %dms\n", result, args->t_die);
			printf("\x1B[31m%d is dead\n\x1B[0m", args->philo[i].id);
			args->deadbool = 1;
			pthread_mutex_unlock(&args->print_mutex);
			pthread_mutex_unlock(args->philo[i].death_mutex);
			return (NULL);
		}
		pthread_mutex_unlock(args->philo[i].death_mutex);
		i++;
		usleep (50);
	}
}

void	*routine(void *arg)
{
	int	eatnum;
	t_philos	*philo;

	philo = (t_philos *)arg;
	eatnum = philo->data->eat_num;
	if ((philo->id % 2) == 0)
		usleep(1000);
	while (1)
	{
		pthread_mutex_lock(&philo->data->counter_mutex);
		if ((eatnum != -1 && philo->counter >= eatnum ))
		{
			pthread_mutex_unlock(&philo->data->counter_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->data->counter_mutex);
		pthread_mutex_lock(philo->death_mutex);
		if (philo->data->deadbool == 1)
		{
			pthread_mutex_unlock(philo->death_mutex);
			break ;
		}
		pthread_mutex_unlock(philo->death_mutex);
		philo_eat(philo);
		pthread_mutex_lock(&philo->data->counter_mutex);
		philo->counter++;
		pthread_mutex_unlock(&philo->data->counter_mutex);
		philo_sleep(philo);
		philo_think(philo);
		usleep(1000);
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
	if (args->philo_num == 1)
	{
		one_philo(args->philo);
		return (1);
	}
	i = 0;
	while (i < args->philo_num)
	{
		pthread_create(&args->philo[i].thread_id, \
NULL, routine, &args->philo[i]);
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
	printf("ended\n");
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
	return (0);
}
