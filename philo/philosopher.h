#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H

# include <pthread.h>

typedef struct s_data t_data;

typedef struct s_philos
{
	int				id;
	int				meals_eaten;
	long			last_meal_time;
	long			current_time;
	pthread_t		thread_id;
	pthread_mutex_t *l_fork;
	pthread_mutex_t *r_fork;
	pthread_mutex_t	*print_status;
	pthread_mutex_t	*death_mutex;
	t_data			*data;
}	t_philos;

typedef struct s_data
{
	int				philo_num;
	int				t_die;
	int				t_sleep;
	int				t_eat;
	int				eat_num;
	int				deadbool;
	long			start_time;
	pthread_mutex_t	print_status;
	pthread_mutex_t	death_mutex;
	t_philos		*philo;
	pthread_mutex_t	*fork;
	pthread_t		monitoring;
}	t_data;

/* parsing */
void	atoi_num(char **av, t_data *args);
int		parsing_check(int ac, char **av, t_data *args);
int		check_num(char **av);
long	get_time_ms(void);

#endif