FLAGS = -g -lpthread

default:
	gcc main.c $(FLAGS) && ./a.out
