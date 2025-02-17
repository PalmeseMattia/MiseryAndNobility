SRC = src/main.c src/print.c src/utils.c

FLAGS = -Wall -Wextra -Werror -g

all :
	gcc $(SRC) -lpthread $(FLAGS) -o philo
