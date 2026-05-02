#include "../includes/philosopher.h"
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
		usleep(3000);
	}
	return (0);
}

static void	take_fork(t_philos *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->r_fork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->l_fork);
		print_status(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->l_fork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->r_fork);
		print_status(philo, "has taken a fork");
	}
}

int	philo_eat(t_philos *philo)
{
	take_fork(philo);
	pthread_mutex_lock(philo->meal_time_mutex);
	philo->last_meal_time = get_time_ms();
	pthread_mutex_unlock(philo->meal_time_mutex);
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
	t_think = (philo->data->t_eat * 2 - philo->data->t_sleep) / 2;
	if (t_think < 0)
		t_think = 0;
	ft_usleep(t_think, philo);
}
