/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpalmese <dpalmese@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 14:49:12 by dpalmese          #+#    #+#             */
/*   Updated: 2025/02/18 14:49:22 by dpalmese         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	lock_forks(t_philosopher *philo)
{
	int	left;
	int	right;

	left = (philo -> id - 1) % philo -> info -> n_threads;
	right = (philo -> id) % philo -> info -> n_threads;
	pthread_mutex_lock(&philo -> info -> forks_locks[right]);
	pthread_mutex_lock(&philo -> info -> forks_locks[left]);
}

void	unlock_forks(t_philosopher *philo)
{
	int	left;
	int	right;

	left = (philo -> id - 1) % philo -> info -> n_threads;
	right = (philo -> id) % philo -> info -> n_threads;
	pthread_mutex_unlock(&philo -> info -> forks_locks[right]);
	pthread_mutex_unlock(&philo -> info -> forks_locks[left]);
}

void	set_forks(t_philosopher *philo, int status)
{
	int	left;
	int	right;

	left = (philo -> id - 1) % philo -> info -> n_threads;
	right = (philo -> id) % philo -> info -> n_threads;
	philo -> info -> forks_status[left] = status;
	philo -> info -> forks_status[right] = status;
}

int	check_forks(t_philosopher *philo, int status)
{
	int	left;
	int	right;
	int	left_status;
	int	right_status;

	left = (philo -> id - 1) % philo -> info -> n_threads;
	right = (philo -> id) % philo -> info -> n_threads;
	left_status = philo -> info -> forks_status[left] == status;
	right_status = philo -> info -> forks_status[right] == status;
	return (left_status && right_status);
}
