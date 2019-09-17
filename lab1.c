#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
int main(int argc, char* argv[])
{
   int i, N, A = 507;
   struct timeval T1,T2;
   long delta_ms;

   N = atoi(argv[1]);
   gettimeofday(&T1, NULL);
   for (i = 0; i < 50; i++) {
      unsigned  int seed = i;
      // generate
      double *M1 = malloc(N * sizeof(double));
      for (int j = 0; j < N; j++) {
         // M1[j] = ((double)rand_r(&seed)/RAND_MAX)/(A - 1) + 1;
         M1[j] = 1 + (double) rand_r(&seed) / (RAND_MAX / (A) + 1);
      }

      double *M2 = malloc(N * sizeof(double));
      for (int j = 0; j < N/2; j++) {
         // M2[j] = ((double)rand_r(&seed)/RAND_MAX)/(9 * A) + A;
         M2[j] = A + (double) rand_r(&seed) / (RAND_MAX / (9 * A + 1) + 1);
      }

      // map
      for (int j = 0; j < N; j++) {
         M1[i] = exp(sqrt(M1[i]));
      }
      for (int j = 0; j < N/2; j++) {
         M2[j] = abs(tan(M2[j] + (j == 0 ? 0 : M2[j-1])));
      }

      // merge
      for (int j = 0; j < N/2; j++) {
         M2[j] = M1[j] > M2[j] ? M2[j] : M1[j];
      }
      
      // sort
      int index = 0; 
      while (index < N/2) { 
         if (index == 0) 
               index++; 
         if (M2[index] >= M2[index - 1]) 
               index++; 
         else { 
               double temp = M2[index]; 
               M2[index] = M2[index - 1]; 
               M2[index - 1] = temp;
               index--; 
         } 
      }

      // reduce
      int z = 0;
      for (int j = 0; j < N/2; j++) {
         z += M2[j];
      }
   }

   gettimeofday(&T2, NULL);
   delta_ms = 1000*(T2.tv_sec - T1.tv_sec) + (T2.tv_usec - T1.tv_usec)/1000;
   printf("\nN=%d. Milliseconds passed: %ld\n", N, delta_ms);

   return 0;
}