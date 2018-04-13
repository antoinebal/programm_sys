#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef void *(*fct_ptr_type)(void *);

static double* px = NULL;

void* lannister(int* max) {
  for(int i=0 ; i < *max ; i++) {
    pthread_mutex_lock(&mutex);
    printf("px -> %lf \n", *px);
    pthread_mutex_unlock(&mutex);
    sleep(1);
  }
}

void main () {
  pthread_t tid;
  int max;
  printf("Choix max: ");
  scanf("%d", &max);
  double x = 1.0;
  px = &x;
  pthread_create(&tid, NULL, (fct_ptr_type)lannister, &max);
 
    for(int i=0 ; i < max ; i++) {
      pthread_mutex_lock(&mutex);
      px=NULL;
      printf("i = %d \n", i);
      px = &x;
      pthread_mutex_unlock(&mutex);
      sleep(1);
    }

}
