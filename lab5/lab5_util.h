#ifndef LAB5_UTIL_H
#define LAB5_UTIL_H

#define NUM_THREADS 4
#define NUM_OF_EXP 50
#define FULL_PROGRES NUM_OF_EXP * 5
#define SCALE ((double) FULL_PROGRES / 100)

typedef struct _thread_data_t {
  int tid;
  double *M1;
  double *M2;
  int start_i;
  int end_i;
} thread_data_t;

void *thr_merge(void *arg);
void *printProgres(void *progres);

thread_data_t buildThreadDataT(int tid, double *M1, double *M2, int start_i, int end_i);

#endif