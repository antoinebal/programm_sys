#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

pthread_once_t once;

void fonction_init() {
  printf("for the win \n");
}


void* f1(void* arg) {
    for(int i=0; i < *(int*)arg; i++) {
      pthread_once(&once,fonction_init);
      printf("got it %d \n", i);
      incrementation();
      sleep(2);
  }
}

int incrementation() {
  static int octopus=0;
  octopus++;
  printf("octopus : %d \n", octopus);
}
  

int main(void) {
  int N=5;
  pthread_t tid1;
  pthread_t tid2;

  pthread_create(&tid1, NULL, f1,(void*)&N);
  pthread_create(&tid2, NULL, f1,(void*)&N);
  
  for(int i=0; i < N; i++) {
    pthread_once(&once,fonction_init);
    printf("got it %d \n", i);
    incrementation();
    sleep(2);
  }
}
  
