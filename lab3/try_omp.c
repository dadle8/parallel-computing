#include <omp.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    int i = 1;
    #pragma omp parallel
    {
        printf("Hello, world!");
        #pragma omp critical
        {   
            i++;
            printf(" i=%d\n", i);
        }
    }

    int a[10];
    #pragma omp parallel for
    for (i = 0; i < 10; ++i)
    {
        a[i] = i*i*i*i*i*i;
    }

    for(i = 0; i < 10; ++i)
    {
        printf("a[%d]=%d\n", i, a[i]);
    }

    int tmp;
    #pragma omp parallel for private(tmp)
    for (i = 0; i < 10; ++i)
    {
        tmp = i*i*i;
        a[i] = tmp*tmp;
    }

    for(i = 0; i < 10; ++i)
    {
        printf("a[%d]=%d\n", i, a[i]);
    }

    int sum = 0;
    #pragma omp parallel for reduction (+:sum)
    for (i = 0; i < 100; i++) sum +=i;

    printf("sum=%d\n", sum);

    float s = 0;
    #pragma omp parallel for reduction (+:s) num_threads(6)
    for (i = 1; i < 1000000; ++i) {
        s += 1.0/i;
    }
    printf("s=%f\n", s);
}
