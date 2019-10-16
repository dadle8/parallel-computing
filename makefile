all: lab3

lab3: ./lab3/lab3.c
	gcc -O2 -Wall -Werror -o ./lab3/lab3-seq ./lab3/lab3.c -lm -fopenmp