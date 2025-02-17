/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpalmese <dpalmese@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 14:31:36 by dpalmese          #+#    #+#             */
/*   Updated: 2024/10/11 14:38:46 by dpalmese         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/philosophers.h"

void	print_eat(t_philosopher *philo)
{
	long long	start;
	int			id;

	start = philo -> info -> start;
	id = philo -> id;
	pthread_mutex_lock(&philo -> info -> write_lock);
	printf("%lld %d is eating\n", (get_milliseconds() - start), id);
	pthread_mutex_unlock(&philo -> info -> write_lock);
}

void	print_think(t_philosopher *philo)
{
	long long	start;
	int			id;

	start = philo -> info -> start;
	id = philo -> id;
	pthread_mutex_lock(&philo -> info -> write_lock);
	printf("%lld %d is thinking\n", (get_milliseconds() - start), id);
	pthread_mutex_unlock(&philo -> info -> write_lock);
}

void	print_sleep(t_philosopher *philo)
{
	long long	start;
	int			id;

	start = philo -> info -> start;
	id = philo -> id;
	pthread_mutex_lock(&philo -> info -> write_lock);
	printf("%lld %d is sleeping\n", (get_milliseconds() - start), id);
	pthread_mutex_unlock(&philo -> info -> write_lock);
}

void	print_fork(t_philosopher *philo)
{
	long long	start;
	int			id;

	start = philo -> info -> start;
	id = philo -> id;
	pthread_mutex_lock(&philo -> info -> write_lock);
	printf("%lld %d has taken a fork\n", (get_milliseconds() - start), id);
	pthread_mutex_unlock(&philo -> info -> write_lock);
}
