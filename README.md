# parallel-computing

# How to compile lab1
> gcc -O2 -Wall -Werror -o lab1-seq lab1.c -lm

> gcc -O2 -Wall -Werror -floop-parallelize-all -ftree-parallelize-loops=K lab1.c -o lab1-par-K -lm

# How to compile lab2
> gcc -m64 -c -IFW_1.3.1_Lin64 lab2.c -lm <br>
> gcc -m64 -LFW_1.3.1_Lin64/lib lab2.o -o lab2 -lfwBase -lfwSignal -lrt -lm

Before run execute
> export LD_LIBRARY_PATH=/home/dadle8/itmo/parallel-computing/lab2/FW_1.3.1_Lin64/lib:$LD_LIBRARY_PATH
