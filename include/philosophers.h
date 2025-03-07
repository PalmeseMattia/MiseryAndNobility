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
#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

# define UP 1
# define DOWN 0
# define TRUE 1
# define FALSE 0

typedef struct s_thread_info
{
	long long		start;
	pthread_mutex_t	*forks_locks;
	pthread_mutex_t	*meals_locks;
	pthread_mutex_t	write_lock;
	pthread_mutex_t	*last_meals_lock;
	int				n_threads;
	unsigned int	time_to_die;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	char			*forks_status;
	char			someone_died;
	int				*n_meals;
	int				max_meals;
}	t_thread_info;

typedef struct s_philosopher
{
	t_thread_info	*info;
	long long		last_meal;
	int				id;
}	t_philos;

void		*ft_calloc(size_t nmemb, size_t size);
// Prints
void		print_eat(t_philos *philo);
void		print_sleep(t_philos *philo);
void		print_think(t_philos *philo);
void		print_fork(t_philos *philo);
// Time util
long long	get_milliseconds(void);
// Monitor routine
void		*stop_simulation(void *arg);
// Mutex utils
void		lock_forks(t_philos *philo);
void		unlock_forks(t_philos *philo);
void		set_forks(t_philos *philo, int status);
int			check_forks(t_philos *philo, int status);
// Actions
void		think(t_philos *philo);
void		p_sleep(t_philos *philo);
void		eat(t_philos *philo);

#endif
