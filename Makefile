SRC =	src/main.c src/print.c src/time_utils.c src/stop_simulation.c \
		src/actions.c src/mutex_utils.c src/ft_calloc.c

FLAGS = -Wall -Wextra -Werror -O3 #-g -gdwarf-4

all :
	gcc $(SRC) -lpthread $(FLAGS) -o philo
