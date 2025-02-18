SRC =	src/main.c src/print.c src/time_utils.c src/stop_simulation.c \
		src/actions.c src/mutex_utils.c src/ft_calloc.c

FLAGS = -Wall -Wextra -Werror
DEBUG = -g -gdwarf-4
OPTIMIZE = -O3

EXEC = philo

all :
	gcc $(SRC) -lpthread $(FLAGS) $(OPTIMIZE) -o $(EXEC)

debug :
	gcc $(SRC) -lpthread $(FLAGS) $(DEBUG) -o $(EXEC)

fclean :
	rm -f $(EXEC)

re : fclean all
