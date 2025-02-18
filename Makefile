SRC =	src/main.c src/print.c src/time_utils.c src/stop_simulation.c \
		src/actions.c src/mutex_utils.c

FLAGS = -Wall -Wextra -Werror -g -gdwarf-4

all :
	gcc $(SRC) -lpthread $(FLAGS) -o philo
