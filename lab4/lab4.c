#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

int main(int argc, char* argv[])
{
   int i, N, lenM1, lenM2, lenM2m1, A = 504;
   double minNotZero = 0.0, X = 0.0, T1,T2, delta_s;

   N = atoi(argv[1]);
   lenM1 = N;
   lenM2 = N / 2;
   lenM2m1 = lenM2 - 1;
   T1 = omp_get_wtime();
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
      #pragma omp parallel default(none) shared(lenM2m1, lenM2, M2) num_threads(2)
      {
         #pragma omp sections 
         {
            #pragma omp section
            {
               for (int j = 0; j < lenM2 / 2 - 1; j++) {
                  int indexMin = j;

                  for (int k = j + 1; k < lenM2 / 2; k++) {
                     if (M2[indexMin] > M2[k])
                     indexMin = k;
                  }

                  if (indexMin != j) {
                     double temp = M2[j];
                     M2[j] = M2[indexMin];
                     M2[indexMin] = temp;
                  }
               }
            }

            #pragma omp section
            {
               for (int j = lenM2 / 2; j < lenM2m1; j++) {
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
            }
         }
      }

      // Merge M2
      double M2merged[lenM2];
      int sz_A = lenM2 / 2, sz_B = lenM2, indexA = 0, indexB = lenM2 / 2, k = 0;
      while (indexA < sz_A && indexB < sz_B) {
        if (M2[indexA] < M2[indexB]) {
            M2merged[k++] = M2[indexA++];        
        } else {
            M2merged[k++] = M2[indexB++];
        }
      }
      // Copy all elements, if needed
      while (indexA < sz_A) {
         M2merged[k++] = M2[indexA++];
      }
      while (indexB < sz_B) {
         M2merged[k++] = M2[indexB++];
      }
      // End Selection sort

      // reduce
      for (int j = 0; j < lenM2; j++) {
         if (M2merged[j] != 0) {
            minNotZero = M2merged[j];
            break;
         }
      }
      #pragma omp parallel for default(none) shared(lenM2, M2merged, minNotZero) reduction (+:X)
      for (int j = 0; j < lenM2; j++) {
         if (((int)(M2merged[j] / minNotZero)) % 2 == 0) {
            X += sin(M2merged[j]);
         }
      }
   }

   T2 = omp_get_wtime();
   delta_s = T2 - T1;
   printf("\nN=%d. Milliseconds passed: %f. X=%f\n", N, delta_s, X);

   return 0;
}
