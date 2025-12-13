#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H

# include <pthread.h>

typedef struct s_philos
{
	int				id;
	int				meals_eaten;
	pthread_mutex_t *l_fork;
	pthread_mutex_t *r_fork;
	pthread_mutex_t	*print_status;
	pthread_mutex_t	*death_mutex;
}	t_philos;

typedef struct s_data
{
	int				philo_num;
	int				t_die;
	int				t_sleep;
	int				t_eat;
	int				eat_num;

	long			start_time;
	int				dead_bool;
	pthread_mutex_t	print_status;
	pthread_mutex_t	death_mutex;
	t_philos		*philo;
	pthread_mutex_t	*fork;
	pthread_t		*thread_id;
}	t_data;

void	atoi_num(char **av, t_data *args);
int		parsing_check(int ac, char **av, t_data *args);
int		ft_atoi(char *s);
int		check_num(char **av);

#endif