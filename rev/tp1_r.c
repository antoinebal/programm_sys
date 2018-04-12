#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef void *(*fct_ptr_type)(void *);

void* lannister(int* N) {
  for(int i=0 ; i < *N ; i++) {
    printf("Lannister of Castral Rock \n");
    sleep(1);
  }
  int *s = malloc(sizeof(int));
  *s = 44;
  pthread_exit(s);
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
  pthread_create(&tid, NULL, (fct_ptr_type)lannister, &N);
 
    for(i=0 ; i < M ; i++) {
      printf("Stark of Winterfell \n");
      sleep(1);
    }

    int* s = NULL;
    pthread_join(tid, (void**)&s);
    printf("%d \n", *s);
  
}
