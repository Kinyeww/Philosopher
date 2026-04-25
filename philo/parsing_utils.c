#include "philosopher.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

static int	ft_atoi(char *s)
{
	long	res;

	res = 0;
	while (*s == ' ' || (*s >= 9 && *s <= 13))
		s++;
	while (*s >= '0' && *s <= '9')
	{
		if (res > (2147483647 - (*s - '0')) / 10)
			return (-1);
		res = (res * 10) + (*s -'0');
		s++;
	}
	return ((int)res);
}

int	check_num(char **av)
{
	int	i;
	int	j;

	i = 1;
	j = 0;
	while (av[i])
	{
		j = 0;
		while (av[i][j])
		{
			if (av[i][j] < '0' || av[i][j] > '9')
				return (0);
			j++;
		}
		if (j == 0)
			return (0);
		i++;
	}
	return (true);
}

int	atoi_num(char **av, t_data *args)
{
	args->philo_num = ft_atoi(av[1]);
	args->t_die = ft_atoi(av[2]);
	args->t_eat = ft_atoi(av[3]);
	args->t_sleep = ft_atoi(av[4]);
	if (args->philo_num < 1 || args->t_die < 0\
		|| args->t_eat < 0 || args->t_sleep < 0)
		return (false);
	if (av[5])
	{
		args->eat_num = ft_atoi(av[5]);
		// printf ("philo num = %d\ntime to die = %d\ntime to sleep = %d\ntime to eat = %d\ntime to eat = %d\n", args->philo_num, args->t_die, args->t_sleep, args->t_eat, args->eat_num);
		return (true);
	}
	else
	{
		args->eat_num = -1;
		// printf ("philo num = %d\ntime to die = %d\ntime to sleep = %d\ntime to eat = %d\ntime to eat = %d(optional)\n", args->philo_num, args->t_die, args->t_sleep, args->t_eat, args->eat_num);
		return (true);
	}
}

int	parsing_check(int ac, char **av, t_data *args)
{
	if (ac < 5 || ac > 6)
	{
		printf("Error, Usage: <num_of_philo> <time_to_die> <time_to_eat> <time_to_sleep> <number_of_times_to_eat>\n");
		return (false);
	}
	if (!(check_num(av)))
	{
		printf("error: invalid num detected\n");
		return (false);
	}
	if (!(atoi_num(av, args)))
	{
		printf("error: argument error\n");
		return (false);
	}
	return (true);
}
