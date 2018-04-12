#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void* lannister(void* arg) {
  int* N = (int*)arg;
  int i = 0;
  for(i=0 ; i < *N ; i++) {
    printf("Lannister of Castral Rock \n");
    sleep(1);
  }
  pthread_exit(1);
}

void main () {
  pthread_t tid;
  int M;
  int N;
  printf("Choix Stark : ");
  scanf("%d", &M);
  printf("Choix Lannister: ");
  scanf("%d", &N);
  int i = 0;
  pthread_create(&tid, NULL, lannister, (void*)&N);
 
    for(i=0 ; i < M ; i++) {
      printf("Stark of Winterfell \n");
      sleep(1);
    }
  
}
