#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>


void handler(int sig) {
  
  printf("on tue le fils");
  exit(0);
}

int main(void)
{

  //CREATION ET BIND DU MASQUE
  sigset_t masque;
  //on remplit le masque (il ne laisse rien passer)
  sigfillset(&masque);
  //on enlève le signal 10 SIGUSR1 (car il doit le traiter)
  sigdelset(&masque, SIGUSR1);
  //on enlève le signal ctrl+c SIGINT (car il doit le traiter)
  sigdelset(&masque, SIGINT);
  //on le bind
  sigprocmask(SIG_SETMASK, &masque, NULL);
  
  int pid;
  
  pid=fork();
 
  if (pid == -1) {
    printf("erreur \n");

    //* FILS *//
  } else if (pid == 0) {
    printf("fils \n");

    //CREATION DU SIGACTION
    struct sigaction action;
    action.sa_handler = handler;
    action.sa_flags = 0;
    sigaction(SIGUSR1, &action, NULL);

    //le fils doit attendre un signal SIGUSR1
    while (1) { pause(); };
    

    //* PERE *//
  } else { 
    printf("pere \n");
    sleep(10);

    //ENVOI DU SIGNAL AU FILS
    kill(pid, SIGUSR1);

    //CREATION DU SIGACTION
    struct sigaction action2;
    action2.sa_handler = handler;
    action2.sa_flags = 0;
     sigaction(SIGINT, &action2, NULL);

    //le père doit attendre le signal du terminal
    while (1) { };
    

    return 0;

  }
}
