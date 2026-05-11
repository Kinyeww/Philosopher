#include "../includes/philosopher.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

static int	check_finished(t_data *args, int i);
static int	check_eatnum(t_philos *philo, int eatnum);

void	*monitoring_thread(t_data *args)
{
	long	result;
	int		i;

	i = 0;
	wait_start(args);
	while (1)
	{
		if (i == args->philo_num)
			i = 0;
		if (args->eat_num != -1 && done_eat_count(args) == args->philo_num)
			return (NULL);
		if (check_finished(args, i) == 1)
		{
			i++;
			continue ;
		}
		pthread_mutex_lock(&args->meal_time_mutex);
		result = get_time_ms() - args->philo[i].last_meal_time;
		pthread_mutex_unlock(&args->meal_time_mutex);
		if (result >= (long)args->philo[i].data->t_die)
			return (print_dead(args, i));
		i++;
		usleep (50);
	}
}

void	*routine(void *philo_arg)
{
	int			eatnum;
	t_philos	*philo;

	philo = (t_philos *)philo_arg;
	eatnum = philo->data->eat_num;
	wait_start(philo->data);
	if ((philo->id % 2) == 0)
		usleep(1000);
	while (1)
	{
		if (check_deadbool(philo) == 1)
			break ;
		if (philo_eat(philo) == 1)
		{
			if (check_eatnum(philo, eatnum) == 1)
				break ;
		}
		else
			break ;
		philo_sleep(philo, eatnum);
		philo_think(philo, eatnum);
	}
	return (NULL);
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
	now = get_time_ms();
	philo->current_time = now - (philo->data->start_time);
	printf("%ld %d %s\n", philo->current_time, philo->id, s);
	pthread_mutex_unlock(philo->death_mutex);
	pthread_mutex_unlock(&philo->data->print_mutex);
}

static int	check_finished(t_data *args, int i)
{
	pthread_mutex_lock(&args->counter_mutex);
	if (args->philo[i].finished == 1)
	{
		pthread_mutex_unlock(&args->counter_mutex);
		return (1);
	}
	pthread_mutex_unlock(&args->counter_mutex);
	return (0);
}

static int	check_eatnum(t_philos *philo, int eatnum)
{
	pthread_mutex_lock(&philo->data->counter_mutex);
	philo->counter++;
	if ((eatnum != -1 && philo->counter >= eatnum))
	{
		philo->finished = 1;
		philo->data->finished_count++;
		pthread_mutex_unlock(&philo->data->counter_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->counter_mutex);
	return (0);
}
