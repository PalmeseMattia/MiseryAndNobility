#include "../include/philosophers.h"

void	think(t_philosopher *philo)
{
	print_think(philo);
	if (philo -> id % 2 == 1 && philo -> info -> n_threads % 2 == 1)
		usleep(philo -> info -> time_to_eat * 1000);
}

void	p_sleep(t_philosopher *philo)
{
	print_sleep(philo);
	usleep(philo -> info -> time_to_sleep * 1000);
}

void	eat(t_philosopher *philo)
{
	while(1)
	{
		lock_forks(philo);
		if (check_forks(philo, DOWN))
		{
			set_forks(philo, UP);
			print_fork(philo);
			unlock_forks(philo);
			print_eat(philo);
			philo -> last_meal = get_milliseconds();
			usleep(philo -> info -> time_to_eat * 1000);
			lock_forks(philo);
			set_forks(philo, DOWN);
			unlock_forks(philo);
			break;
		}
		else
		{
			unlock_forks(philo);
			usleep(100);
		}
	}
}