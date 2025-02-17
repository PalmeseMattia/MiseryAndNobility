/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpalmese <dpalmese@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 14:32:16 by dpalmese          #+#    #+#             */
/*   Updated: 2024/10/11 14:41:40 by dpalmese         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef PHILO_H
# define PHILO_H

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#define RIGHT(n, size) ((n + 1) % size)
#define LEFT(n) n
#define UP 1
#define DOWN 0

typedef struct s_thread_info
{
	long long		start;
	pthread_mutex_t	*forks_locks;
	pthread_mutex_t	write_lock;
	int				n_threads;
	unsigned int	time_to_die;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	char			*forks_status;
	char			someone_died;
}	t_thread_info;

typedef struct s_philosopher
{
	t_thread_info	*info;
	long long		last_meal;
	int				id;
}	t_philosopher;

// Prints
void		print_eat(t_philosopher *philo);
void		print_sleep(t_philosopher *philo);
void		print_think(t_philosopher *philo);
void		print_fork(t_philosopher *philo);
// Time util
long long	get_milliseconds();
#endif
