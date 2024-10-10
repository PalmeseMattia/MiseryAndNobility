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
	pthread_mutex_t	*forks_locks;
	long long		start;
	char			*forks_status;
	int				n_threads;
}	t_thread_info;

typedef struct s_philosopher
{
	t_thread_info	*info;
	int				id;
	long long		last_meal;
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

void	think(t_philosopher *philo)
{
	printf("%lld %d is thinking\n", (get_milliseconds() - philo -> info -> start), philo -> id);
}

void	p_sleep(useconds_t usecs, t_philosopher *philo)
{
	printf("%lld %d is sleeping\n", (get_milliseconds() - philo -> info -> start), philo -> id);
	usleep(usecs);
}

void	eat(useconds_t usecs, t_philosopher *philo)
{
	int	left;
	int	right;

	left = (philo -> id - 1) % philo -> info -> n_threads;
	right = (philo -> id) % philo -> info -> n_threads;
	//printf("ID: %d LEFT: %d RIGHT: %d\n", philo -> id, left, right);
	//	Try to eat:
	while(1)
	{
		// Lock (RIGHT)
		pthread_mutex_lock(&philo -> info -> forks_locks[right]);
		// If (RIGHT is DOWN)
		if (philo -> info -> forks_status[right] == DOWN)
		{
			// Lock (LEFT)
			pthread_mutex_lock(&philo -> info -> forks_locks[left]);
			// If (LEFT is DOWN)
			if (philo -> info -> forks_status[left] == DOWN)
			{
				// FORKS UP
				philo -> info -> forks_status[left] = UP;
				philo -> info -> forks_status[right] = UP;
				printf("%lld %d has taken a fork\n", (get_milliseconds() - philo -> info -> start), philo -> id);
				printf("%lld %d has taken a fork\n", (get_milliseconds() - philo -> info -> start), philo -> id);
				// UNLOCK STATUSES
				pthread_mutex_unlock(&philo -> info -> forks_locks[left]);
				pthread_mutex_unlock(&philo -> info -> forks_locks[right]);
				// EAT
				printf("%lld %d is eating\n", (get_milliseconds() - philo -> info -> start), philo -> id);
				philo -> last_meal = get_milliseconds();
				usleep(usecs);
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
				// Unlock LEFT
				pthread_mutex_unlock(&philo -> info -> forks_locks[left]);
				// Unlock RIGHT
				pthread_mutex_unlock(&philo -> info -> forks_locks[right]);
			}
		}
		// Else
		else
		{
			// Unlock RIGHT
			pthread_mutex_unlock(&philo -> info -> forks_locks[right]);
			// Wait a bit
			usleep(100);
		}
	}
	// REPEAT
}

void	*hello(void *arg)
{
	while (1)
	{
		eat(300000, ((t_philosopher *)arg));
		p_sleep(100000, ((t_philosopher *)arg));
		think((t_philosopher *)arg);
	}
	return NULL;
}


int main(int argc, char *argv[])
{
	t_thread_info	info;
	t_philosopher	*philosophers;
	pthread_t		*threads;
	int				res;

	if (argc < 2) {
		printf("Please specify the number of threads to spawn!\n");
		exit(EXIT_FAILURE);
	}
	info.n_threads = atoi(argv[1]);

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
	
	// Create threads
	threads = (pthread_t *)calloc(info.n_threads, sizeof(pthread_t));
	for (int i = 0; i < info.n_threads; i++) {
		philosophers[i].info = &info;
		philosophers[i].id = i + 1;
		philosophers[i].last_meal = get_milliseconds();
		res = pthread_create(threads + i, NULL, &hello, philosophers + i);
		if (res != 0) {
			perror("Pthread create failed!");
			free(threads);
			exit(EXIT_FAILURE);
		}
	}
	
	// Now join threads
	for (int i = 0; i < info.n_threads; i++)
	{
		pthread_join(threads[i], NULL);
	}
	free(threads);
	free(info.forks_locks);
	free(info.forks_status);
}
