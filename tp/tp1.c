#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>


void handler(int sig) {
  
  printf("on tue le fils \n");
  exit(0);
}



void handlerPere(int sig) {
  printf("on tue le pere");
  exit(0);
}

int main(void)
{

 
  
  int pid;
  
  pid=fork();
 
  if (pid == -1) {
    printf("erreur \n");

    //* FILS *//
  } else if (pid == 0) {
    printf("fils %d \n", pid);

    //CREATION DU SIGACTION
    struct sigaction action;
    action.sa_handler = handler;
    action.sa_flags = 0;
    sigaction(SIGUSR1, &action, NULL);

    //le fils doit attendre un signal SIGUSR1
    while (1) { pause(); };
    

    //* PERE *//
  } else { 
    printf("pere %d \n", pid);

    //CREATION DU SIGACTION POUR REAGIR A LA MORT DU FILS
    struct sigaction action2;
    action2.sa_handler = handlerMortFils;
    action2.sa_flags = 0;
    sigaction(SIGCHLD, &action2, NULL);

    //CREATION DU SIGACTION POUR TRAITER LE SIGNAL SIGUSR1 DEPUIS LE TERMINAL
    struct sigaction action3;
    action3.sa_handler = handlerPere;
    action3.sa_flags = 0;
    sigaction(SIGUSR1, &action3, NULL);


    
    sleep(3);




    //ENVOI DU SIGNAL AU FILS
    kill(pid, SIGUSR1);

    
    //le père doit attendre le signal du terminal
    while (1) { pause(); };
    

    return 0;

  }
}
