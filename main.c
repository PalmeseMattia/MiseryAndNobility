/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpalmese <dpalmese@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 12:49:47 by dpalmese          #+#    #+#             */
/*   Updated: 2024/10/11 14:21:03 by dpalmese         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
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

long long	get_milliseconds()
{
	struct timeval	tv;
	int				time_res;

	time_res = gettimeofday(&tv, NULL);
	if (time_res != 0)
	{
		printf("Error getting time of the day!\n");
		exit(EXIT_FAILURE);
	}
	else
		return (((long long)tv.tv_sec)*1000)+(tv.tv_usec/1000);
}

void	print_eat(t_philosopher *philo)
{
	pthread_mutex_lock(&philo -> info -> write_lock);
	printf("%lld %d is eating\n", (get_milliseconds() - philo -> info -> start), philo -> id);
	pthread_mutex_unlock(&philo -> info -> write_lock);
}

void	print_think(t_philosopher *philo)
{
	pthread_mutex_lock(&philo -> info -> write_lock);
	printf("%lld %d is thinking\n", (get_milliseconds() - philo -> info -> start), philo -> id);
	pthread_mutex_unlock(&philo -> info -> write_lock);
}

void	print_sleep(t_philosopher *philo)
{
	pthread_mutex_lock(&philo -> info -> write_lock);
	printf("%lld %d is sleeping\n", (get_milliseconds() - philo -> info -> start), philo -> id);
	pthread_mutex_unlock(&philo -> info -> write_lock);
}

void	print_fork(t_philosopher *philo)
{
	pthread_mutex_lock(&philo -> info -> write_lock);
	printf("%lld %d has taken a fork\n", (get_milliseconds() - philo -> info -> start), philo -> id);
	pthread_mutex_unlock(&philo -> info -> write_lock);
}

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
	int	left;
	int	right;

	left = (philo -> id - 1) % philo -> info -> n_threads;
	right = (philo -> id) % philo -> info -> n_threads;
	//	Try to eat:
	while(1)
	{
		pthread_mutex_lock(&philo -> info -> forks_locks[right]);
		pthread_mutex_lock(&philo -> info -> forks_locks[left]);
		// If (FORKS ARE DOWN)
		if (philo -> info -> forks_status[right] == DOWN && philo -> info -> forks_status[left] == DOWN)
		{
			// FORKS UP
			philo -> info -> forks_status[left] = UP;
			philo -> info -> forks_status[right] = UP;
			print_fork(philo);
			// UNLOCK STATUSES
			pthread_mutex_unlock(&philo -> info -> forks_locks[left]);
			pthread_mutex_unlock(&philo -> info -> forks_locks[right]);
			// EAT
			print_eat(philo);
			philo -> last_meal = get_milliseconds();
			usleep(philo -> info -> time_to_eat * 1000);
			// LOCK STATUSES
			pthread_mutex_lock(&philo -> info -> forks_locks[left]);
			pthread_mutex_lock(&philo -> info -> forks_locks[right]);
			// FORKS DOWN
			philo -> info -> forks_status[left] = DOWN;
			philo -> info -> forks_status[right] = DOWN;
			// UNLOCK STATUSES
			pthread_mutex_unlock(&philo -> info -> forks_locks[left]);
			pthread_mutex_unlock(&philo -> info -> forks_locks[right]);
			// BREAK LOOP
			break;
		}
		// Else
		else
		{
			pthread_mutex_unlock(&philo -> info -> forks_locks[left]);
			pthread_mutex_unlock(&philo -> info -> forks_locks[right]);
			usleep(100);
		}
	}
	// REPEAT
}

void	*hello(void *arg)
{
	while (1)
	{
		eat((t_philosopher *)arg);
		p_sleep((t_philosopher *)arg);
		think((t_philosopher *)arg);
	}
	return (NULL);
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
		id = (id + 1) % philos[id].info -> n_threads;
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

	// Get Time
	info.start = get_milliseconds();
	
	// Allocate philosophers
	philosophers = (t_philosopher *)calloc(info.n_threads, sizeof(t_philosopher));

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
		res = pthread_create(threads + i, NULL, &hello, philosophers + i);
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
