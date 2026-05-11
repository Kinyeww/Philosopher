#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H

# include <pthread.h>

typedef struct	s_data	t_data;

typedef struct s_philos
{
	int				id;
	int				counter;
	int				finished;
	long			last_meal_time;
	long			current_time;
	pthread_t		thread_id;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*meal_time_mutex;
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
	int				ready;
	int				deadbool;
	long			start_time;
	int				finished_count;
	pthread_mutex_t	death_mutex;
	pthread_mutex_t	meal_time_mutex;
	pthread_mutex_t	counter_mutex;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	ready_mutex;
	t_philos		*philo;
	pthread_mutex_t	*fork;
	pthread_t		monitoring;
}	t_data;

/* actions */
int		ft_usleep(int time_to_sleep, t_philos *philo);
int		philo_eat(t_philos *philo);
void	philo_sleep(t_philos *philo, int eatnum);
void	philo_think(t_philos *philo, int eatnum);

/* parsing */
int		atoi_num(char **av, t_data *args);
int		parsing_check(int ac, char **av, t_data *args);
int		check_num(char **av);

/* init_clean */
void	print_status(t_philos *philo, char *s);
void	philo_init(t_data *args);
int		start_sim(t_data *args);
void	cleanup(t_data *args);


/* routine_helper */
int		check_deadbool(t_philos *philo);
int		done_eat_count(t_data *args);
long	get_time_ms(void);
void	wait_start(t_data *args);
void	*print_dead(t_data *args, int i);

/* routine */
void	*monitoring_thread(t_data *arg);
void	*routine(void *arg);
void	print_status(t_philos *philo, char *s);
void	one_philo(t_philos *philo);

#endif