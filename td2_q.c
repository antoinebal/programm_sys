#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int * px=NULL;
pthread_mutex_t mutex;

void * courroux(void* arg)
{
 
  for(int ind=0; ind < (int)arg; ind++)
    {
      pthread_mutex_lock(&mutex);
      printf("%d \n",*px);
      pthread_mutex_unlock(&mutex);
    }
 
  return 0;
}

int main(void)
{
  pthread_t tid;
  int x=1;
  int max;
  px = &x;
  scanf("%d", &max);
   pthread_create(&tid, NULL, courroux,(void*)&max);
  
  for (int j=0; j<max; j++)
    {
      pthread_mutex_lock(&mutex);
      px=NULL;
      printf("%d \n",j);  
      px = &x;
      pthread_mutex_unlock(&mutex);
    }

  pthread_join(tid, 0);
  return 0;

}
