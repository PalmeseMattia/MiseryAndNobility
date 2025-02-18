/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stop_simulation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpalmese <dpalmese@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 16:04:14 by dpalmese          #+#    #+#             */
/*   Updated: 2025/02/18 16:06:57 by dpalmese         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

int	check_meals(t_philos philo)
{
	int	id;

	id = 0;
	while (id < philo.info->n_threads)
	{
		pthread_mutex_lock(&philo.info->meals_locks[id]);
		if (philo.info->n_meals[id] < philo.info->max_meals)
		{
			pthread_mutex_unlock(&philo.info->meals_locks[id]);
			return (FALSE);
		}
		pthread_mutex_unlock(&philo.info->meals_locks[id]);
		id++;
	}
	pthread_mutex_lock(&philo.info->write_lock);
	printf("Number of meals raggiunto\n");
	return (TRUE);
}

void	*stop_simulation(void *arg)
{
	t_philos		*philos;
	int				id;
	long long		time;
	long long		death;
	int				odd;

	id = 0;
	philos = (t_philos *)arg;
	while (1)
	{
		time = get_milliseconds();
		if (time - philos[id].last_meal > philos[id].info -> time_to_die)
		{
			pthread_mutex_lock(&philos[0].info -> write_lock);
			death = time - philos[id].info -> start;
			printf("%lld %d died\n", death, id + 1);
			break ;
		}
		odd = id % 2 == 0;
		if (philos[id].info->max_meals > 0 && odd && check_meals(philos[id]))
			break ;
		id = (id + 1) % philos[id].info -> n_threads;
	}
	return (NULL);
}
