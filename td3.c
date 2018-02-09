#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int ressource = 4;
int * Res = &ressource;
pthread_mutex_t mutex;
pthread_cond_t condition;

void * f1(void* arg) {
  
  int i = *(int*) arg;
  printf("avant lock %d \n", i);  
  //pthread_mutex_lock(&mutex);

  while (1) {

  pthread_cond_wait(&condition, &mutex);

  //while (*Res < i) {  };
  if (Res < i) {
  Res-=i;
 
  sleep(4); //pour simuler le travail du thread
  }
  Res += i;
  pthread_mutex_unlock(&mutex);

  pthread_cond_broadcast(&condition);
  }
}

int main (void) {
  pthread_t tid;
  int i1=1;
  int i2=2;
  int iMain=3;

  pthread_create(&tid, NULL, f1, (void*)&i1);
  pthread_create(&tid, NULL, f1, (void*)&i2);

  printf("avant lock %d \n", iMain);
  
  //pthread_mutex_lock(&mutex);

  

  //while (Res < iMain) { printf("bloque %d \n", iMain); };
  while (1) {

    pthread_cond_wait(&condition, &mutex);

    if (Res < iMain) {
      Res-=iMain;
      sleep(4); //pour simuler le travail du thread
    }
  //pthread_mutex_unlock(&mutex);

    Res += iMain;
    pthread_mutex_unlock(&mutex);

    pthread_cond_broadcast(&condition);
  }
}
