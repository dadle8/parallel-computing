#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <omp.h>

int main(int argc, char* argv[])
{
   int i, N, lenM1, lenM2, lenM2m1, A = 504;
   struct timeval T1,T2;
   long delta_ms;
   double minNotZero = 0.0, X = 0.0;

   N = atoi(argv[1]);
   lenM1 = N;
   lenM2 = N / 2;
   lenM2m1 = lenM2 - 1;
   gettimeofday(&T1, NULL);
   for (i = 0; i < 50; i++) {
      minNotZero = 0.0;
      unsigned  int seed = i;
      // generate
      double M1[lenM1];
      for (int j = 0; j < lenM1; j++) {
         M1[j] = 1 + (double)(rand_r(&seed)) / (RAND_MAX / (A));
      }
      double M2[lenM2];
      for (int j = 0; j < lenM2; j++) {
         M2[j] = A + (double)(rand_r(&seed)) / (RAND_MAX / (9 * A + 1));
      }

      // map
      #pragma omp parallel for default(none) shared(lenM1, M1)
      for (int j = 0; j < lenM1; j++) {
         M1[j] = exp(sqrt(M1[j]));
      }
      double previousValue = 0.0;
      for (int j = 0; j < lenM2; j++) {
         double currentValue = M2[j];
         M2[j] = fabs(tan(previousValue + currentValue));
         previousValue = currentValue;
      }

      // merge
      #pragma omp parallel for default(none) shared(lenM2, M1, M2)
      for (int j = 0; j < lenM2; j++) {
         M2[j] = M1[j] > M2[j] ? M2[j] : M1[j];
      }

      // Selection sort
      for (int j = 0; j < lenM2m1; j++) {
         int indexMin = j;

         for (int k = j + 1; k < lenM2; k++) {
            if (M2[indexMin] > M2[k])
            indexMin = k;
         }

         if (indexMin != j) {
            double temp = M2[j];
            M2[j] = M2[indexMin];
            M2[indexMin] = temp;
         }
      }

      // reduce
      for (int j = 0; j < lenM2; j++) {
         if (M2[j] != 0) {
            minNotZero = M2[j];
            break;
         }
      }
      #pragma omp parallel for default(none) shared(lenM2, M2, minNotZero) reduction (+:X)
      for (int j = 0; j < lenM2; j++) {
         if (((int)(M2[j] / minNotZero)) % 2 == 0) {
            X += sin(M2[j]);
         }
      }
   }

   gettimeofday(&T2, NULL);
   delta_ms = 1000*(T2.tv_sec - T1.tv_sec) + (T2.tv_usec - T1.tv_usec)/1000;
   printf("\nN=%d. Milliseconds passed: %ld. X=%f\n", N, delta_ms, X);

   return 0;
}
