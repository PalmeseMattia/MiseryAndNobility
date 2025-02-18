/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpalmese <dpalmese@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 16:03:25 by dpalmese          #+#    #+#             */
/*   Updated: 2025/02/18 16:03:52 by dpalmese         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	think(t_philos *philo)
{
	print_think(philo);
	if (philo -> id % 2 == 1 && philo -> info -> n_threads % 2 == 1)
		usleep(philo -> info -> time_to_eat * 1000);
}

void	p_sleep(t_philos *philo)
{
	print_sleep(philo);
	usleep(philo -> info -> time_to_sleep * 1000);
}

static void	p_eat(t_philos *philo)
{
	print_eat(philo);
	philo -> last_meal = get_milliseconds();
	usleep(philo -> info -> time_to_eat * 1000);
	pthread_mutex_lock(&philo->info->meals_locks[philo->id -1]);
	philo->info->n_meals[philo->id - 1]++;
	pthread_mutex_unlock(&philo->info->meals_locks[philo->id -1]);
}

void	eat(t_philos *philo)
{
	while (1)
	{
		lock_forks(philo);
		if (check_forks(philo, DOWN))
		{
			set_forks(philo, UP);
			print_fork(philo);
			unlock_forks(philo);
			p_eat(philo);
			lock_forks(philo);
			set_forks(philo, DOWN);
			unlock_forks(philo);
			break ;
		}
		else
		{
			unlock_forks(philo);
			usleep(100);
		}
	}
}
