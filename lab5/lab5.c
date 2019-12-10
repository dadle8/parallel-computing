#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <pthread.h>
#include "lab5_util.h"

#ifdef _OPENMP
   #include "omp.h"

   void printResults(int N, double delta_s, double delta_merge, double X) {
      printf("\nN=%d. Milliseconds passed: %f. X=%f. Delta merge=%f\n", N, 1000.0 * delta_s, X, 1000 * delta_merge);
   }
#else
   int omp_get_wtime(){
      struct timeval T11;
      gettimeofday(&T11, NULL);
      return T11.tv_sec * 1000 + T11.tv_usec/1000;
   }

   void printResults(int N, double delta_s, double delta_merge, double X) {
      printf("\nN=%d. Milliseconds passed: %f. X=%f. Delta merge=%f\n", N, delta_s, X, delta_merge);
   }

   int omp_get_thread_num() {
      return 1;
   }
#endif

int progres = 0;

int main(int argc, char* argv[])
{
   int rc;
   pthread_t t_progres_id;
   pthread_t thr[NUM_THREADS];
   thread_data_t thr_data[NUM_THREADS];

   if ((rc = pthread_create(&t_progres_id, NULL, printProgres, &progres))) {
      printf("Error: pthread_create, rc: %d\n", rc);
      return EXIT_FAILURE;
   }

   int N = atoi(argv[1]),
       lenM1 = N,
       lenM2 = N / 2,
       A = 504;
   double minNotZero = 0.0,
          T1, T2, T_start_pregenerate, T_end_pregenerate, T_start_merge, T_end_merge,
          delta_rand_r = 0.0, delta_s, delta_merge = 0.0,
          X = 0.0;

   T1 = omp_get_wtime();
   for (int i = 0; i < NUM_OF_EXP; i++) {
      unsigned int seed = i;

      // Start Pregenerate values
      int rand_v1[lenM1];
      int rand_v2[lenM2];
      T_start_pregenerate = omp_get_wtime();
      for(int j = 0; j < lenM1; j++) {
         rand_v1[j] = rand_r(&seed);
      }
      for(int j = 0; j < lenM2; j++) {
         rand_v2[j] = rand_r(&seed);
      }
      T_end_pregenerate = omp_get_wtime();
      delta_rand_r += T_end_pregenerate - T_start_pregenerate;
      // End Pregenerate values

      // Start Generate
      double M1[lenM1];
      #pragma omp parallel for default(none) shared(lenM1, M1, rand_v1, A)
      for (int j = 0; j < lenM1; j++) {
         // printf("thread num in generate M1 = %d\n", omp_get_thread_num());
         M1[j] = 1 + (double)(rand_v1[j]) / (RAND_MAX / (A));
      }
      double M2[lenM2];
      #pragma omp parallel for default(none) shared(lenM2, M2, rand_v2, A)
      for (int j = 0; j < lenM2; j++) {
         // printf("thread num in generate M2 = %d\n", omp_get_thread_num());
         M2[j] = A + (double)(rand_v2[j]) / (RAND_MAX / (9 * A + 1));
      }
      // End Generate
      progres++;

      // Start Map
      #pragma omp parallel for default(none) shared(lenM1, M1)
      for (int j = 0; j < lenM1; j++) {
         // printf("thread num in map = %d\n", omp_get_thread_num());
         M1[j] = exp(sqrt(M1[j]));
      }
      double previousValue = 0.0;
      for (int j = 0; j < lenM2; j++) {
         double currentValue = M2[j];
         M2[j] = fabs(tan(previousValue + currentValue));
         previousValue = currentValue;
      }
      // End Map
      progres++;

      // Start Merge
      T_start_merge = omp_get_wtime();
      int start_i = 0, end_i = lenM2 / NUM_THREADS;
      if (lenM2 < NUM_THREADS) {
         end_i = lenM2;
      }
      /* create threads */
      for (int j = 0; j < NUM_THREADS; ++j) {
         thr_data[j] = buildThreadDataT(j, M1, M2, start_i, end_i);

         // printf("thr_data tid=%d, start_i=%d, end_i=%d\n", thr_data[j].tid, thr_data[j].start_i, thr_data[j].end_i);

         if ((rc = pthread_create(&thr[j], NULL, thr_merge, &thr_data[j]))) {
            printf("Error: pthread_create, rc: %d\n", rc);
            return EXIT_FAILURE;
         }

         if (lenM2 < NUM_THREADS) {
            break;
         }
         start_i = end_i;
         end_i = end_i + lenM2 / NUM_THREADS;

         if (j == NUM_THREADS - 2) {
            end_i = lenM2;
         }
      }
      /* block until all threads complete */
      for (int j = 0; j < NUM_THREADS; ++j) {
         pthread_join(thr[j], NULL);
      }
      T_end_merge = omp_get_wtime();
      delta_merge += T_end_merge - T_start_merge;
      // End Merge
      progres++;

      // Start Selection sort
      #pragma omp parallel default(none) shared(lenM2, M2) num_threads(2)
      {
         #pragma omp sections
         {
            #pragma omp section
            {
               // printf("thread num in first section = %d\n", omp_get_thread_num());
               for (int j = 0; j < lenM2 / 2 - 1; j++) {
                  // printf("thread num in first for = %d\n", omp_get_thread_num());
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
               // printf("thread num in second section = %d\n", omp_get_thread_num());
               for (int j = lenM2 / 2; j < lenM2 - 1; j++) {
                  // printf("thread num in second for = %d\n", omp_get_thread_num());
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
      progres++;

      // printf("thread num before reduce = %d\n", omp_get_thread_num());

      // Start Reduce
      for (int j = 0; j < lenM2; j++) {
         if (M2merged[j] != 0) {
            minNotZero = M2merged[j];
            break;
         }
      }
      #pragma omp parallel for default(none) shared(lenM2, M2merged, minNotZero) reduction (+:X)
      for (int j = 0; j < lenM2; j++) {
         // printf("thread num in last for = %d\n", omp_get_thread_num());
         if (((int)(M2merged[j] / minNotZero)) % 2 == 0) {
            X += sin(M2merged[j]);
         }
      }
      // End Reduce
      progres++;
   }

   T2 = omp_get_wtime();
   delta_s = T2 - T1 - delta_rand_r;
   
   /* Wait progres bar thread */
   pthread_join(t_progres_id, NULL);

   printResults(N, delta_s, delta_merge, X);

   return 0;
}
