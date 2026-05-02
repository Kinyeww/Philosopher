#include "../includes/philosopher.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

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
	return (0);
}
