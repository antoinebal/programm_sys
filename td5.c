#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

pthread_mutex_t mutex;


void unlock_mutex(void * tt) {
  pthread_mutex_unlock(&mutex);
}


void* f1(void* arg) {
  pthread_mutex_lock(&mutex);
  printf("j'ai le mutex \n");
  pthread_cleanup_push(unlock_mutex, NULL);
  for(int i=0; i < *(int*)arg; i++) {
    printf("%d \n", (int)pthread_self());
    sleep(1);
  }
  pthread_mutex_unlock(&mutex);     
}




int main(void) {
  int max=10;
  pthread_t tid;
  
  
  for (int no = 0; no < 2; no++) {
    pthread_create(&tid, NULL, f1,(void*)&max);
    
    for(int i=0; i < max/2; i++) {
      printf("main %d \n", (int)pthread_self());
      sleep(1);
    }
    pthread_cancel(tid);
    sleep(1);
  }
  return 0;
}
