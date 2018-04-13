#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



typedef void *(*fct_ptr_type)(void *);

typedef struct T_Seiche{
  int* iterations;
  int ressources_requises;
}


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

//notre compteur de ressources
static int* px = NULL;

  
void* greyjoy(T_Seiche theon) {
  
  for(int i=0 ; i < *theon.iterations ; i++) {
    pthread_cond_wait(&cond, &mutex);
    
    if (*px >= theon.ressources_requises_requises) {//assez de ressources
      printf("Thread n° %d prend ses ressources", theon.ressources_requises);
      *px = *px - theon.ressources_requises;
      printf("%d", *px);  
      pthread_cond_broadcast(&cond);
      pthread_mutex_unlock(&mutex);
      
      sleep(1);

      pthread_cond_wait(&cond, &mutex);
      printf("Thread n° %d libère ses ressources", theon.ressources_requises);
      *px = *px + theon.ressources_requises;
      printf("%d", *px);
      pthread_cond_broadcast(&cond);
      pthread_mutex_unlock(&mutex);
    } else {//pas suffisamment de ressources
      pthread_cond_broadcast(&cond);
      pthread_mutex_unlock(&mutex);
    }
  }
}



//la Main du Roi utilise trois ressources
void main () {
  pthread_t tid;
  int nbre_it;
  printf("Choix nombre itérations: ");
  scanf("%d", &nbre_it);

  //création des threads
  for(int j=0 ; j < 2 ; j++) {
    T_Seiche asha;
    asha.iterations = &nbre_it;
    asha.ressources_requises = j+1;
    pthread_create(&tid, NULL, (fct_ptr_type)greyjoy, &asha);
    printf("Thread n° %d crée", j+1);
  }

  
  for(int i=0 ; i < *nbre_it ; i++) {
    pthread_cond_wait(&cond, &mutex);
    
    if (*px >= 3) {//assez de ressources
      printf("Thread n°3 prend ses ressources");
      *px = *px - 3;
      printf("%d", *px);  
      pthread_cond_broadcast(&cond);
      pthread_mutex_unlock(&mutex);
      
      sleep(1);

      pthread_cond_wait(&cond, &mutex);
      printf("Thread n°3 libère ses ressources");
      *px = *px + 3;
      printf("%d", *px);
      pthread_cond_broadcast(&cond);
      pthread_mutex_unlock(&mutex);
    } else {//pas suffisamment de ressources
      pthread_cond_broadcast(&cond);
      pthread_mutex_unlock(&mutex);
    }
  }


}
