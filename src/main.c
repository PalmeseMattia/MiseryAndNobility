/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpalmese <dpalmese@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 12:49:47 by dpalmese          #+#    #+#             */
/*   Updated: 2024/10/11 14:43:20 by dpalmese         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/philosophers.h"

void	*main_loop(void *arg)
{
	while (1)
	{
		eat((t_philosopher *)arg);
		p_sleep((t_philosopher *)arg);
		think((t_philosopher *)arg);
	}
	return (NULL);
}

static void	initialize_info(t_thread_info *info, int argc, char *argv[])
{
	int	i;

	i = 0;
	if (argc < 5)
		exit(EXIT_FAILURE);
	info->n_threads = atoi(argv[1]);
	info->time_to_die = atoi(argv[2]);
	info->time_to_eat = atoi(argv[3]);
	info->time_to_sleep = atoi(argv[4]);
	info->someone_died = 0;
	if (argc > 5)
		info->max_meals = atoi(argv[5]);
	else
		info->max_meals = 0;
	info->start = get_milliseconds();
	info->n_meals = ft_calloc(info->n_threads, sizeof(int));
	info->forks_locks = ft_calloc(info->n_threads, sizeof(pthread_mutex_t));
	info->forks_status = ft_calloc(info->n_threads, sizeof(char));
	while (i < info->n_threads)
	{
		pthread_mutex_init(&info->forks_locks[i], NULL);
		i++;
	}
	pthread_mutex_init(&info->write_lock, NULL);
}

static void	init_philos(t_philosopher *philos, pthread_t *threads, t_thread_info *info)
{
	pthread_attr_t	attr;
	int				res;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	for (int i = 0; i < info->n_threads; i++) {
		philos[i].info = info;
		philos[i].id = i + 1;
		philos[i].last_meal = get_milliseconds();
		res = pthread_create(threads + i, &attr, &main_loop, philos + i);
		if (res != 0) {
			perror("Pthread create failed!");
			free(threads);
			exit(EXIT_FAILURE);
		}
	}
}

int main(int argc, char *argv[])
{
	t_thread_info	info;
	t_philosopher	*philosophers;
	pthread_t		*threads;
	pthread_t		monitor;

	initialize_info(&info, argc, argv);	
	philosophers = ft_calloc(info.n_threads, sizeof(t_philosopher));
	threads = ft_calloc(info.n_threads, sizeof(pthread_t));
	init_philos(philosophers, threads, &info);
	pthread_create(&monitor, NULL, &stop_simulation, philosophers);
	pthread_join(monitor, NULL);
	free(threads);
	free(info.forks_locks);
	free(info.forks_status);
}