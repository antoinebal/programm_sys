#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#define L 256


int main()
{
	int tube1[2] , tube2[2] ; 
	int pid ; 
	char msg[L] ;
	pipe(tube1) ; 
	pipe(tube2) ; 
	switch (pid = fork())
	{
		case -1: printf("Erreur de création du fils\n") ;
			 return -1 ; 
	       	
		case 0:  printf("\nJe suis le fils et mon PID est : %d\n" , getpid()) ; 
			 close(tube2[0]) ; 
			 close(tube1[1]) ; 
			 printf("%d : Fermeture de la sortie du tube2 pour le fils\n", getpid()) ;
			 printf("%d : Fermeture de l'entrée du tube1 pour le fils\n", getpid()) ; 
			 read(tube1[0], msg, L) ; 
			 printf("Message reçu : %s\n" , msg) ;
			 break; 
		
		default: printf("Je suis le père et mon PID est : %d\n" , getpid()) ;
			 printf("%d : Création de tube1\n", getpid()) ;
			 close(tube1[0]) ; 
			 close(tube2[1]) ; 
			 printf("%d : Fermeture de la sortie du tube1 pour le père\n", getpid()) ;
			 printf("%d : Envoi d'un message au fils\n" , getpid()) ;  
			 sprintf(msg, "Je suis ton père\n") ; 
			 write(tube1[1] , msg, L) ; 
			 break ;  	 	 		 
	}
	return 0; 
} 
