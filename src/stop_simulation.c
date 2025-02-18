#include "../include/philosophers.h"

void	*stop_simulation(void *arg)
{
	t_philosopher	*philos;
	int				id;

	id = 0;
	philos = (t_philosopher *)arg;
	while (1)
	{
		if (get_milliseconds() - philos[id].last_meal > philos[id].info -> time_to_die)
		{
			pthread_mutex_lock(&philos[0].info -> write_lock);
			printf("%lld %d died\n", get_milliseconds() - philos[id].info -> start, id + 1);
			break;
		}
		id = (id + 1) % philos[id].info -> n_threads;
	}
	return (NULL);
}