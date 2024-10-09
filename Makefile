SRC = main.c

FLAGS = -Wall -Wextra -Werror -g

all :
	gcc main.c -lpthread $(FLAGS) -o philo
