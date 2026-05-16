#include "philosopher.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>

int	ft_usleep(int time_to_sleep, t_philos *philo)
{
	long	start;

	start = get_time_ms();
	while ((get_time_ms() - start) < time_to_sleep)
	{
		if (check_deadbool(philo) == 1)
			return (1);
		usleep(250);
	}
	return (0);
}

static void	take_fork(t_philos *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->r_fork);
		pthread_mutex_lock(philo->l_fork);
	}
	else
	{
		pthread_mutex_lock(philo->l_fork);
		pthread_mutex_lock(philo->r_fork);
	}
}

int	philo_eat(t_philos *philo)
{
	long	now;

	take_fork(philo);
	pthread_mutex_lock(philo->meal_time_mutex);
	now = get_time_ms();
	if (now - philo->last_meal_time >= (long)philo->data->t_die)
	{
		pthread_mutex_unlock(philo->meal_time_mutex);
		pthread_mutex_unlock(philo->l_fork);
		pthread_mutex_unlock(philo->r_fork);
		return (0);
	}
	philo->last_meal_time = now;
	pthread_mutex_unlock(philo->meal_time_mutex);
	print_status(philo, "has taken a fork");
	print_status(philo, "has taken a fork");
	print_status(philo, "is eating");
	if (ft_usleep(philo->data->t_eat, philo) == 1)
	{
		pthread_mutex_unlock(philo->l_fork);
		pthread_mutex_unlock(philo->r_fork);
		return (0);
	}
	pthread_mutex_unlock(philo->l_fork);
	pthread_mutex_unlock(philo->r_fork);
	return (1);
}

void	philo_sleep(t_philos *philo, int eatnum)
{
	pthread_mutex_lock(&philo->data->counter_mutex);
	if ((eatnum != -1 && philo->counter >= eatnum))
	{
		pthread_mutex_unlock(&philo->data->counter_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->data->counter_mutex);
	print_status(philo, "is sleeping");
	ft_usleep(philo->data->t_sleep, philo);
}

void	philo_think(t_philos *philo, int eatnum)
{
	long	t_think;

	pthread_mutex_lock(&philo->data->counter_mutex);
	if ((eatnum != -1 && philo->counter >= eatnum))
	{
		pthread_mutex_unlock(&philo->data->counter_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->data->counter_mutex);
	print_status(philo, "is thinking");
	if (philo->data->philo_num % 2 == 0)
		return ;
	pthread_mutex_lock(&philo->data->meal_time_mutex);
	t_think = (philo->data->t_die - (get_time_ms() - philo->last_meal_time)
			- philo->data->t_eat) / 2;
	pthread_mutex_unlock(&philo->data->meal_time_mutex);
	if (t_think < 0)
		t_think = 0;
	ft_usleep(t_think, philo);
}
