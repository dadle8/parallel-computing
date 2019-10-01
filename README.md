# parallel-computing

# How to compile lab2
> gcc -m64 -c -IFW_1.3.1_Lin64 lab2.c -lm <br>
> gcc -m64 -LFW_1.3.1_Lin64/lib lab2.o -o lab2 -lfwBase -lfwSignal -lrt -lm

Before run execute
> export LD_LIBRARY_PATH=/home/dadle8/itmo/parallel-computing/lab2/FW_1.3.1_Lin64/lib:$LD_LIBRARY_PATH
