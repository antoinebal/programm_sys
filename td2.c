#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

double* px = NULL;

void * crawford(void* arg)
{
  for(int ind=0; ind < *(int*)arg; ind++)
    {
      printf("%lf", *px);
      printf("\n");
      printf("lolilol");
      // sleep(1);
      
    }
  return 0;
}


int main(void)
{
  pthread_t tid;
  double x=1.0;
  px = &x;
  int Max=6;
  
  pthread_create(&tid, NULL, crawford,(void*)&Max);

  for (int i=0; i<Max; i++)
    {
      px = NULL;
       printf("%d",i);
      printf("\n");
      px=&x;
      // sleep(1);
    }
  return 0;
  }
