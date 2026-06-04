#include "philosopher.h"

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

void	take_fork(t_philos *philo)
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

int	set_meal_time(t_philos *philo)
{
	long	now;

	pthread_mutex_lock(&philo->meal_time_mutex);
	now = get_time_ms();
	if (now - philo->last_meal_time >= (long)philo->data->t_die)
	{
		pthread_mutex_unlock(&philo->meal_time_mutex);
		pthread_mutex_unlock(philo->l_fork);
		pthread_mutex_unlock(philo->r_fork);
		return (0);
	}
	philo->last_meal_time = now;
	pthread_mutex_unlock(&philo->meal_time_mutex);
	return (1);
}

long	get_time_ms(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	philo_stagger(t_philos *philo)
{
	if (philo->data->philo_num % 2 == 0)
	{
		if (philo->id % 2 == 0)
			ft_usleep(philo->data->t_eat / 2, philo);
	}
	else
	{
		if (philo->id % 2 == 0)
			ft_usleep(philo->data->t_eat, philo);
	}
}
