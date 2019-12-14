all: lab1 lab2 lab3 lab4 lab5

# Compile sections
lab1: ./lab1/lab1-seq.c ./lab1/lab1-par-2.c ./lab1/lab1-par-3.c ./lab1/lab1-par-4.c ./lab1/lab1-par-6.c ./lab1/lab1-par-8.c
	gcc -O2 -Wall -Werror -o lab1-seq lab1.c -lm
	gcc -O2 -Wall -Werror -floop-parallelize-all -ftree-parallelize-loops=2 lab1.c -o lab1-par-2 -lm
	gcc -O2 -Wall -Werror -floop-parallelize-all -ftree-parallelize-loops=3 lab1.c -o lab1-par-3 -lm
	gcc -O2 -Wall -Werror -floop-parallelize-all -ftree-parallelize-loops=4 lab1.c -o lab1-par-4 -lm
	gcc -O2 -Wall -Werror -floop-parallelize-all -ftree-parallelize-loops=6 lab1.c -o lab1-par-6 -lm
	gcc -O2 -Wall -Werror -floop-parallelize-all -ftree-parallelize-loops=8 lab1.c -o lab1-par-8 -lm

lab2: ./lab2/lab2.c
	gcc -m64 -c -O2 -Wall -Werror -I./lab2/FW_1.3.1_Lin64 ./lab2/lab2.c -lm
	mv ./lab2.o ./lab2/
	gcc -m64 -L./lab2/FW_1.3.1_Lin64/lib ./lab2/lab2.o -o ./lab2/lab2 -lfwBase -lfwSignal -lrt -lm
	rm -rf ./lab2/lab2.o

lab3: ./lab3/lab3.c ./lab3/lab3_with_schedule.c
	gcc -O2 -Wall -Werror -o ./lab3/lab3 ./lab3/lab3.c -lm -fopenmp
	gcc -O2 -Wall -Werror -o ./lab3/lab3_with_schedule ./lab3/lab3_with_schedule.c -lm -fopenmp

lab4: ./lab4/lab4.c
	gcc -O2 -Wall -Werror -o ./lab4/lab4 ./lab4/lab4.c -lm -fopenmp
	gcc -O2 -Wall -o ./lab4/lab4-without-omp ./lab4/lab4.c -lm

lab5: ./lab5/lab5.c
	gcc -O2 -Wall -Werror -o ./lab5/lab5 ./lab5/lab5.c ./lab5/lab5_util.c -lm -fopenmp

# Clear sections
clear-all: clear-lab1 clear-lab2 clear-lab3 clear-lab4 clear-lab5

clear-lab1: ./lab1/lab1-seq ./lab1/lab1-par-2 ./lab1/lab1-par-3 ./lab1/lab1-par-4 ./lab1/lab1-par-6 ./lab1/lab1-par-8
	rm -rf ./lab1/lab1-seq 
	rm -rf ./lab1/lab1-par-2 
	rm -rf ./lab1/lab1-par-3 
	rm -rf ./lab1/lab1-par-4 
	rm -rf ./lab1/lab1-par-6 
	rm -rf ./lab1/lab1-par-8

clear-lab2: ./lab2/lab2
	rm -rf ./lab2/lab2

clear-lab3: ./lab3/lab3 ./lab3/lab3_with_schedule
	rm -rf ./lab3/lab3
	rm -rf ./lab3/lab3_with_schedule

clear-lab4: ./lab4/lab4
	rm -rf ./lab4/lab4

clear-lab5: ./lab5/lab5
	rm -rf ./lab5/lab5