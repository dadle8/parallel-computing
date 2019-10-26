all: lab1 lab2 lab3 lab4

#lab1:

#lab2: ./lab2/lab2.c

lab3: ./lab3/lab3.c ./lab3/lab3-schedule.c
	gcc -O2 -Wall -Werror -o ./lab3/lab3 ./lab3/lab3.c -lm -fopenmp
	gcc -O2 -Wall -Werror -o ./lab3/lab3-schedule ./lab3/lab3-schedule.c -lm -fopenmp

lab4: ./lab4/lab4.c
	gcc -O2 -Wall -Werror -o ./lab4/lab4 ./lab4/lab4.c -lm -fopenmp

clear: ./lab3/lab3 ./lab3/lab3-schedule ./lab4/lab4
	rm -rf ./lab3/lab3
	rm -rf ./lab3/lab3-schedule
	rm -rf ./lab4/lab4
