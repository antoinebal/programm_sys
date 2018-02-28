#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void * courroux(void* arg)
{
  for(int ind=0; ind < *(int*)arg; ind++)
    {
      printf("et mon courroux\n");
      sleep(1);
    }
  return 0;
  //pthread_exit(11);
}


void * colere(void* arg)
{
  for(int ind=0; ind < *(int*)arg; ind++)
    {
      printf("et ma colere\n");
      sleep(1);
    }
  return 0;
  //pthread_exit(11);
}


int main(void)
{

  pthread_t tid;
  int N;
  int M;
  scanf("%d", &N);
  scanf("%d", &M);
  pthread_create(&tid, NULL, courroux,(void*)&N);
  for (int j=0; j<M; j++)
    {
      printf("coucou\n");
      sleep(1);
    }

  pthread_cancel(tid);
  printf("tid cancelé");

  pthread_t tid2;
  pthread_create(&tid2, NULL, colere,(void*)&N);
   for (int j=0; j<M; j++)
    {
      printf("aurevoir\n");
      sleep(1);
    }

  return 0;


  /* int** receptacle;
  int receptacle2;
  receptacle2=pthread_join(tid, receptacle);
  printf("%d", **receptacle);*/



 
  
  }
  

//compiler : gcc -Wall -pthread /home/abalayss/prog_sys/td1_q.c
//executer : ./a.out

/*il faut que je comprenne la 1.3*/
