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
	pthread_mutex_lock(philo->print_status);
	printf("Philo %d %s", philo->id, s);
	pthread_mutex_unlock(philo->print_status);
}

// void	death_mutex(t_data *args)
// {
// 	int	i;

// 	pthread_mutex_lock(philo->death_mutex);
// 	i = 0;

// }

int	is_alive(t_philos *philo)
{
	int	current;
	int	compare;
	int	result;

	pthread_mutex_lock(philo->death_mutex);
	current = get_time_ms();
	compare = philo->last_meal_time;
	result = current - compare;
	if (result > philo->data->t_die)
	{
		pthread_mutex_unlock(philo->death_mutex);
		return (0);
	}
	else
	{
		pthread_mutex_unlock(philo->death_mutex);
		return (1);
	}
}

void	*routine(t_philos *philo)
{
	printf("Hi,I am philo %d\n", philo->id);
	if ((philo->id % 2) == 0)
		usleep(1000);
	while (is_alive(philo))
	{

		printf("omg im alive, my id is %d\n", philo->id);
		philo->counter++;
	}
	if (!is_alive(philo))
	{
		printf("gg im dead, my id is %d\n", philo->id);
		printf("id = %d, printed times = %d\n", philo->id, philo->counter);
	}
	return (NULL);
}

int	get_time_ms(void)
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
	i = 0;
	while (i < args->philo_num)
	{
		pthread_join(args->philo[i].thread_id, NULL);
		i++;
	}
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
