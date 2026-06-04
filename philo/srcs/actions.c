#include "philosopher.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>

int	philo_eat(t_philos *philo)
{
	take_fork(philo);
	if (!(set_meal_time(philo)))
		return (0);
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
	pthread_mutex_lock(&philo->meal_time_mutex);
	t_think = (philo->data->t_die - (get_time_ms() - philo->last_meal_time)
			- philo->data->t_eat) / 2;
	pthread_mutex_unlock(&philo->meal_time_mutex);
	if (t_think < 0)
		t_think = 0;
	ft_usleep(t_think, philo);
}
