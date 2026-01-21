#include "philosopher.h"
#include <unistd.h>

void	philo_eat(t_philos *philo)
{
	long	elapsed_time;
	long	start_time;

	pthread_mutex_lock(philo->l_fork);
	pthread_mutex_lock(philo->r_fork);
	elapsed_time = get_time_ms();
	start_time = get_time_ms();
	print_status(philo, "has taken a fork\n\n");
	print_status(philo, "has taken a fork\n\n");
	print_status(philo, "is eating\n\n");
	while (elapsed_time - start_time < philo->data->t_eat)
	{
		pthread_mutex_lock(philo->death_mutex);
		if (philo->data->deadbool == 1)
		{
			pthread_mutex_unlock(philo->death_mutex);
			pthread_mutex_unlock(philo->l_fork);
			pthread_mutex_unlock(philo->r_fork);
			return ;
		}
		pthread_mutex_unlock(philo->death_mutex);
		usleep(100);
		elapsed_time = get_time_ms();
	}
	pthread_mutex_lock(philo->meal_time_mutex);
	philo->last_meal_time = get_time_ms();
	pthread_mutex_unlock(philo->meal_time_mutex);
	pthread_mutex_unlock(philo->l_fork);
	pthread_mutex_unlock(philo->r_fork);
}

void	philo_sleep(t_philos *philo)
{
	long	start_time;
	long	elapsed_time;

	start_time = get_time_ms();
	elapsed_time = get_time_ms();
	print_status(philo, "is sleeping\n\n");
	while (elapsed_time - start_time < philo->data->t_sleep)
	{
		pthread_mutex_lock(philo->death_mutex);
		if (philo->data->deadbool == 1)
		{
			pthread_mutex_unlock(philo->death_mutex);
			return ;
		}
		pthread_mutex_unlock(philo->death_mutex);
		usleep(100);
		elapsed_time = get_time_ms();
	}
}

void	philo_think(t_philos *philo)
{
	pthread_mutex_lock(philo->death_mutex);
	if (philo->data->deadbool == 1)
	{
		pthread_mutex_unlock(philo->death_mutex);
		return ;
	}
	pthread_mutex_unlock(philo->death_mutex);
	print_status(philo, "is thinking\n\n");
}
