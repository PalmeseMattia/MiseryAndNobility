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

int	check_meals(t_philosopher philo)
{
	int	id;

	id = 1;
	while (id <= philo.info->n_threads)
	{
		if (philo.info->n_meals[id] < philo.info->max_meals)
			return FALSE;
		id++;
	}
	pthread_mutex_lock(&philo.info->write_lock);
	printf("Number of meals raggiunto\n");
	return TRUE;
}

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
		if (philos[id].info->max_meals > 0 && check_meals(philos[id]))
			break;
		id = (id + 1) % philos[id].info -> n_threads;
	}
	return (NULL);
}
