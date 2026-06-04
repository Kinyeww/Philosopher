*This project has been created as part of the 42 curriculum by ckin-yew.*

# Philosophers

## Description

Philosophers is a 42 project about threads and mutexes.

The program simulates philosophers sitting around a table. Each philosopher needs two forks to eat. Since the forks are shared, mutexes are used to protect them and avoid data races.

The goal of this project is to manage several threads at the same time, avoid deadlocks, and correctly detect when a philosopher dies or when all philosophers have eaten enough.

## Instructions

### Compilation

To compile the program, run:

```
make
```

This will create the executable:

```
./philo
```

To remove object files:

```
make clean
```

To remove object files and the executable:

```
make fclean
```

To rebuild the project:

```
make re
```

### Execution

Run the program with:

```
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

Example:

```
./philo 5 800 200 200
```

Example with the optional eating count:

```
./philo 5 800 200 200 7
```

Arguments:

* `number_of_philosophers`: number of philosophers and forks.
* `time_to_die`: time in milliseconds before a philosopher dies if he does not eat.
* `time_to_eat`: time in milliseconds a philosopher spends eating.
* `time_to_sleep`: time in milliseconds a philosopher spends sleeping.
* `number_of_times_each_philosopher_must_eat`: optional. If provided, the simulation stops when all philosophers have eaten at least this many times.

## Resources

References used for this project:

Philosophers subject PDF - 42 project page
pthread_create - Linux manual page
pthread_join - Linux manual page
pthread_mutex_init - Linux manual page
pthread_mutex_lock - The Open Group POSIX documentation
gettimeofday - Linux manual page
usleep - Linux manual page

AI assistance was used to help explain thread and mutex behavior, review possible edge cases.
