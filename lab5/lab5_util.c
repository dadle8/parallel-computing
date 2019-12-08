#include "lab5_util.h"
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void *thr_merge(void *arg) {
   thread_data_t *data = (thread_data_t *)arg;

   // printf("data tid=%d, start_i=%d, end_i=%d", data->tid, data->start_i, data->end_i);

  for (int j = data->start_i; j < data->end_i; j++) {
         // printf("thread num in merge = %d\n", data->tid);
         data->M2[j] = data->M1[j] > data->M2[j] ? data->M2[j] : data->M1[j];
   }
 
  pthread_exit(NULL);
}

void *printProgres(void *progres) {
    int *prog = (int *)progres;
   for(;*prog != FULL_PROGRES;) {
      printf("\r%d", (int) (*prog / SCALE));
      fflush(stdout);
      sleep(1);
   }
   printf("\r%d\n", (int) (*prog / SCALE));
   pthread_exit(NULL);
}

thread_data_t buildThreadDataT(int tid, double *M1, double *M2, int start_i, int end_i) {
    thread_data_t thr_data;

    thr_data.tid = tid;
    thr_data.start_i = start_i;
    thr_data.end_i = end_i;
    thr_data.M1 = M1;
    thr_data.M2 = M2;

    return thr_data;
}