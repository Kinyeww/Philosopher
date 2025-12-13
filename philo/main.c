#include "philosopher.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>



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
		pthread_mutex_init(&args->fork[i], NULL);
		args->philo[i].l_fork = &args->fork[i];
		args->philo[i].r_fork = &args->fork[(i + 1) % args->philo_num];
		args->philo[i].print_status = &args->print_status;
		args->philo[i].death_mutex = &args->death_mutex;
		i++;
	}
	printf("done init\n");
}

void	*print_status(t_philos *philo, char *s)
{
	pthread_mutex_lock(philo->print_status);
	printf("Philo %d %s", philo_id, s);
}

// void	*routine

int	start_sim(t_data *args)
{
	pthread_t		*philos; //thread
	pthread_mutex_t	*forks;
	int				i;

	philos = malloc(sizeof(pthread_t) * args->philo_num);
	forks = malloc(sizeof(pthread_mutex_t) * (args->philo_num));
	philo_init(args);
	i = 0;
	while (i < args->philo_num)
	{
		if (args->philo_num == 1)
			return (0);
		pthread_create(&philos[i], NULL, routine, NULL);
		i++;
	}
	i = 0;
	while (i < args->philo_num)
		pthread_join(philos[i++], NULL);
}

int	main(int ac, char **av)
{
	t_data	args;

	if (!parsing_check(ac, av, &args))
	{
		perror("parsing error");
		return (1);
	}
	if (!(start_sim(&args)))
	{
		perror("start simulation error");
		return (1);
	}
	return (0);
}

void	atoi_num(char **av, t_data *args)
{
	args->philo_num = ft_atoi(av[1]); // already guaranteed num
	args->t_die = ft_atoi(av[2]);
	args->t_sleep = ft_atoi(av[3]);
	args->t_eat = ft_atoi(av[4]);
	if (av[5])
	{
		args->eat_num = ft_atoi(av[5]);
		printf ("philo num = %d\ntime to die = %d\ntime to sleep = %d\ntime to eat = %d\ntimes to eat = %d\n", args->philo_num, args->t_die, args->t_sleep, args->t_eat, args->eat_num);
	}
	else
	{
		args->eat_num = -1;
		printf ("philo num = %d\ntime to die = %d\ntime to sleep = %d\ntime to eat = %d\ntimes to eat = optional\n", args->philo_num, args->t_die, args->t_sleep, args->t_eat);
	}
}

int	parsing_check(int ac, char **av, t_data *args)
{
	if (ac < 5 || ac > 6)
	{
		printf("Error, Usage: <num_of_philo> <time_to_die> <time_to_eat> <time_to_sleep> <number_of_times_to_eat>\n");
		return (0);
	}
	if (!(check_num(av)))
	{
		printf("error: invalid num detected\n");
		return (0);
	}
	atoi_num(av, args);
	return (1);
}
