all: lab1 lab2 lab3 lab4

#lab1:

#lab2: ./lab2/lab2.c

lab3: ./lab3/lab3_first_version.c ./lab3/lab3_second_version.c ./lab3/lab3_first_schedule.c ./lab3/lab3_second_schedule.c
	gcc -O2 -Wall -Werror -o ./lab3/lab3_first_version ./lab3/lab3_first_version.c -lm -fopenmp
	gcc -O2 -Wall -Werror -o ./lab3/lab3_second_version ./lab3/lab3_second_version.c -lm -fopenmp
	gcc -O2 -Wall -Werror -o ./lab3/lab3_first_schedule ./lab3/lab3_first_schedule.c -lm -fopenmp
	gcc -O2 -Wall -Werror -o ./lab3/lab3_second_schedule ./lab3/lab3_second_schedule.c -lm -fopenmp

lab4: ./lab4/lab4.c
	gcc -O2 -Wall -Werror -o ./lab4/lab4 ./lab4/lab4.c -lm -fopenmp

clear: ./lab3/lab3_first_version ./lab3/lab3_second_version ./lab3/lab3_first_schedule ./lab3/lab3_second_schedule ./lab4/lab4
	rm -rf ./lab3/lab3_first_version
	rm -rf ./lab3/lab3_second_version
	rm -rf ./lab3/lab3_first_schedule
	rm -rf ./lab3/lab3_second_schedule
	rm -rf ./lab4/lab4
