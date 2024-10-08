#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct s_thread_info
{
	pthread_mutex_t	*forks_locks;
	char			*forks_status;
	int				n_threads;
}	t_thread_info;

typedef struct s_philosopher
{
	t_thread_info	*info;
	int				id;
}	t_philosopher;

void	think(useconds_t usecs, int id)
{
	printf("Philosopher %d is thinking. Big brain time!\n", id);
	usleep(usecs);
}

void	p_sleep(useconds_t usecs, int id)
{
	printf("Philosopher %d is sleeping. Zzzz!\n", id);
	usleep(usecs);
}

void	eat(useconds_t usecs, int id)
{
	//	Try to eat:
	//	Lock (RIGHT)
	//	If (RIGHT is DOWN)
	//		Lock (LEFT)
	//		If (LEFT is DOWN)
	//			FORKS UP
	//			UNLOCK STATUSES
	//			EAT
	//			LOCK STATUSES
	//			FORKS DOWN
	//			UNLOCK STATUSES
	//		Else
	//			Unlock LEFT
	//			Unlock RIGHT
	//	Else
	//		Unlock RIGHT
	//		Wait a bit
	//		REPEAT
	printf("Philosopher %d is putting spaghetti in his pockets.\n", id);
	usleep(usecs);
}

void	*hello(void *arg)
{
	while (1)
	{
		eat(300000, ((t_philosopher *)arg) -> id);
		// Think()
		think(100000, ((t_philosopher *)arg) -> id);
		// Sleep()
		p_sleep(100000, ((t_philosopher *)arg) -> id);
	}
	printf("Hello from thread %d\n", ((t_philosopher *)arg) -> id);
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

	// Allocate philosophers
	philosophers = (t_philosopher *)calloc(info.n_threads, sizeof(t_philosopher));

	// Create threads
	threads = (pthread_t *)calloc(info.n_threads, sizeof(pthread_t));
	for (int i = 0; i < info.n_threads; i++) {
		philosophers[i].info = &info;
		philosophers[i].id = i + 1;
		res = pthread_create(threads + i, NULL, &hello, philosophers + i);
		if (res != 0) {
			perror("Pthread create failed!");
			free(threads);
			exit(EXIT_FAILURE);
		}
	}
	
	// Create mutexes for the state of a fork
	info.forks_locks = (pthread_mutex_t *)calloc(info.n_threads, sizeof(pthread_mutex_t));
	info.forks_status = (char *)calloc(info.n_threads, sizeof(char));

	for (int i = 0; i < info.n_threads; i++)
	{
		pthread_mutex_init(info.forks_locks + i, NULL);
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
