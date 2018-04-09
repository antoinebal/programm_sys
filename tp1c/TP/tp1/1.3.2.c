
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>

int main()
{
	int pid ; 
	switch (pid = fork())
	{
		case -1: printf("Erreur de création du fils\n") ;
			 return -1 ; 
	       	
		case 0:  printf("\nJe suis le fils et mon PID est : %d\n" , getpid()) ; 
			 break; 
		
		default: printf("Je suis le père et mon PID est : %d\n" , getpid()) ;
			 break ;  	 	 		 
	}
	return 0; 
} 
