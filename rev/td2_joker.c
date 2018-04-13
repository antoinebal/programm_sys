#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutexPX = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexPY = PTHREAD_MUTEX_INITIALIZER;

typedef void *(*fct_ptr_type)(void *);

static double* px = NULL;
static double* py = NULL;

  //Lannister manipule px
void* lannister(int* max) {
  for(int i=0 ; i < *max ; i++) {
    pthread_mutex_lock(&mutexPX);
    printf("px -> %lf \n", *px);
    pthread_mutex_unlock(&mutexPX);
    sleep(1);
  }
}

//Baratheon manipule py
  void* baratheon(int* max) {
  for(int i=0 ; i < *max ; i++) {
    pthread_mutex_lock(&mutexPY);
    printf("py -> %lf \n", *py);
    pthread_mutex_unlock(&mutexPY);
    sleep(1);
  }
}


//la Main du Roi manipule px et py
void main () {
  pthread_t tid;
  int max;
  printf("Choix max: ");
  scanf("%d", &max);
  double x = 1.0;
  px = &x;
  double y = 2.0;
  py = &y;
  
    pthread_create(&tid, NULL, (fct_ptr_type)lannister, &max);
    pthread_create(&tid, NULL, (fct_ptr_type)baratheon, &max);
    
    for(int i=0 ; i < max ; i++) {
      pthread_mutex_lock(&mutexPX);
      px=NULL;
      printf("i = %d \n", i);
      px = &x;
      pthread_mutex_unlock(&mutexPX);

      pthread_mutex_lock(&mutexPY);
      py=NULL;
      printf("i = %d \n", i);
      py = &y;
      pthread_mutex_unlock(&mutexPY);
      sleep(1);
    }

}
