#include "philosopher.h"
#include <unistd.h>
#include <sys/time.h>
#include <time.h>

int	ft_usleep(int time_to_sleep, t_philos *philo)
{
	long	start;

	start = get_time_ms();
	while ((get_time_ms() - start) < time_to_sleep)
	{
		pthread_mutex_lock(philo->death_mutex);
		if (philo->data->deadbool == 1)
		{
			pthread_mutex_unlock(philo->death_mutex);
			return (1);
		}
		pthread_mutex_unlock(philo->death_mutex);
		usleep(500);
	}
	return (0);
}

int	philo_eat(t_philos *philo)
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
	pthread_mutex_lock(philo->meal_time_mutex);
	philo->last_meal_time = get_time_ms();
	pthread_mutex_unlock(philo->meal_time_mutex);
	print_status(philo, "is eating");
	if (ft_usleep(philo->data->t_eat, philo) == 1)
	{
		pthread_mutex_unlock(philo->l_fork);
		pthread_mutex_unlock(philo->r_fork);
		return (1);
	}
		pthread_mutex_unlock(philo->l_fork);
		pthread_mutex_unlock(philo->r_fork);
		return (0);
}

void	philo_sleep(t_philos *philo)
{
	print_status(philo, "is sleeping");
	ft_usleep(philo->data->t_sleep, philo);
}

void	philo_think(t_philos *philo)
{
	print_status(philo, "is thinking");
}

void	one_philo(t_philos *philo)
{
	pthread_mutex_lock(philo->l_fork);
	print_status(philo, "has taken a fork");
	ft_usleep(philo->data->t_die, philo);
	print_status(philo, "died");
	pthread_mutex_unlock(philo->l_fork);
}
