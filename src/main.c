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

int main(int argc, char *argv[])
{
	t_thread_info	info;
	t_philosopher	*philosophers;
	pthread_t		*threads;
	pthread_t		monitor;
	int				res;

	if (argc < 5) {
		printf("Please specify all arguments! Usage:\n");
		printf("./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep>\n");
		exit(EXIT_FAILURE);
	}
	info.n_threads = atoi(argv[1]);
	info.time_to_die = atoi(argv[2]);
	info.time_to_eat = atoi(argv[3]);
	info.time_to_sleep = atoi(argv[4]);
	info.someone_died = 0;
	if (argc > 5)
		info.max_meals = atoi(argv[5]);
	else
		info.max_meals = 0;

	// Get Time
	info.start = get_milliseconds();
	
	// Allocate philosophers
	philosophers = (t_philosopher *)calloc(info.n_threads, sizeof(t_philosopher));
	// Allocate array of meals
	info.n_meals = (int *)calloc(info.n_threads, sizeof(int));

	// Create mutexes for the state of a fork
	info.forks_locks = (pthread_mutex_t *)calloc(info.n_threads, sizeof(pthread_mutex_t));
	info.forks_status = (char *)calloc(info.n_threads, sizeof(char));
	for (int i = 0; i < info.n_threads; i++)
	{
		pthread_mutex_init(&info.forks_locks[i], NULL);
	}
	
	// Create mutex to write
	pthread_mutex_init(&info.write_lock, NULL);
	
	// Create threads
	threads = (pthread_t *)calloc(info.n_threads, sizeof(pthread_t));
	for (int i = 0; i < info.n_threads; i++) {
		philosophers[i].info = &info;
		philosophers[i].id = i + 1;
		philosophers[i].last_meal = get_milliseconds();
		// TODO: create with detach attribute
		res = pthread_create(threads + i, NULL, &main_loop, philosophers + i);
		if (res != 0) {
			perror("Pthread create failed!");
			free(threads);
			exit(EXIT_FAILURE);
		}
	}
	
	pthread_create(&monitor, NULL, &stop_simulation, philosophers);
	// Now join threads
	pthread_join(monitor, NULL);
	free(threads);
	free(info.forks_locks);
	free(info.forks_status);
}
