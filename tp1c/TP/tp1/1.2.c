#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>

int pid ;
struct sigaction saCHLD , saUSR1, saINT ; 
sigset_t mask ; /* définition du mask */
	
void finPere (int signum) 
{
	printf("\n") ; 
	printf("%d s'arrête car Victor a balancé le contrôle-C\n" , getpid()) ;
	exit(0) ; 
}

void handler (int signum)  
{
	printf("%d a bien reçu le SIGUSR1(%d) du père\n", getpid() ,signum) ;
	printf("SIGCHLD envoyé.. \n") ; 
	printf("Mort du fils.. \n") ; 
}

void mortFils (int signum)
{
	printf("%d se suspend car il a reçu le SIGCHLD(%d), il attend son interruption.\n", getpid() , signum) ; 
	sigsuspend(&mask) ; 
}


int main()
{
	//on remplit le masque (il bloquera tous les signaux)	
	sigfillset(&mask) ; 

	//on dit au masque de na pas bloquer les signaux SIGUSR1
	sigdelset(&mask , SIGUSR1) ;

	//on dit au masque de na pas bloquer les signaux SIGCHLD
	sigdelset(&mask , SIGCHLD) ; 

	//on dit au masque de na pas bloquer les signaux SIGINT
	sigdelset(&mask , SIGINT) ; 

	//on associe le masque
	sigprocmask(SIG_SETMASK , &mask, NULL) ; 
	
	//PARAMETRAGE DES SIGACTIONS
	//le sigaction saCHLD est associé à la fonction mortFils
	saCHLD.sa_handler = mortFils ;
	//et on lui donne le masque
	saCHLD.sa_mask = mask ; 
	saCHLD.sa_flags = 0 ; 

	//même chose avec saUSR1
	saUSR1.sa_handler = handler ; 
	saUSR1.sa_mask = mask ; 
	saUSR1.sa_flags = 0 ;

	//même chose avec saINT
	saINT.sa_handler = finPere ; 	
	saINT.sa_mask = mask ; 
	saINT.sa_flags = 0 ;

	switch (pid = fork())
	{
				
		case -1: printf("Erreur de création du fils\n") ;
			break ; 
		case 0: printf("Je suis le fils et mon PID est : %d\n" , getpid()) ;
			//signal(SIGUSR1 , handler) ; 
			//on associe le sigaction saUSR1 avec le signal SIGUSR1
			sigaction(SIGUSR1, &saUSR1, NULL) ; 
			sigsuspend(&mask) ;
			break ; 
		default: printf("Je suis le père et mon PID est : %d\n" , getpid()) ; 
			 //signal(SIGINT, finPere) ;
        		 //signal(SIGCHLD, mortFils) ; 
			 sigaction(SIGCHLD, &saCHLD, NULL) ; 
			 sigaction(SIGINT, &saINT, NULL) ; 
			 sleep(1) ; 
			 kill(pid,SIGUSR1) ; 
			 printf("SIGUSR1 envoyé.. \n") ; 
			 sigsuspend(&mask) ; 
			 break ;  
	}
	return 0; 
} 
