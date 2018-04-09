#include <sys/time.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#define L sizeof(int)  

void handler_fils (int sig)
{
	//printf("%d a reçu le SIGINT(%d)\n" , getpid(), sig) ;
	exit(0) ;   
}

/*Dans un tube : 0 <=> lecture
				 1 <=> écriture */

/* le fils écrit dans le tube2 et lit dans le tube1
	le père lit dans le tube1 et écrit dans le tube2 */
float calcul_débit_tube(int nb_ping_pong, char *unit, double *time)
{
	int tube1[2] , tube2[2] ; 
	int pid ;
	float d ; 
	int a ; 
	struct sigaction saINT ;
	clock_t deb, fin ; 
	sigset_t mask ;

	//on crée les tubes
	pipe(tube1) ; 
	pipe(tube2) ;

	//MASQUE
	sigfillset(&mask) ; //on le remplit 
	sigdelset(&mask, SIGINT) ; //on elève SIGINT
	sigprocmask(SIG_SETMASK, &mask, NULL) ; //on le set

	//SIGACTION
	saINT.sa_handler = handler_fils ;
	saINT.sa_mask = mask ; 
	saINT.sa_flags = 0 ;
 
	switch (pid = fork())
	{
		case -1: printf("Erreur de création du fils\n") ;
			 return -1 ; 
	       
		//fils : CONGRATULATIONS FATHER 
		case 0:  close(tube2[0]) ; 
			 close(tube1[1]) ; 
			 sigaction(SIGINT, &saINT, NULL) ; 
			 while (1) 
			 {
				//A N'EST PAS INIT CAR ON LE LIT (en le filant en ref)
			 	read(tube1[0], &a, L) ;
				//printf("%d : a = %d\n" , getpid() , a) ;
				//if (a == nb_ping_pong*2)
				//{
					//break ;
				//}
			 	a += 1 ; 
				
							 	
				write(tube2[1] , &a , L); 
			 }
			 break ;


	    //père : YOU'LL ALWAYS BE MY FIRST BORN 
		default: close(tube1[0]) ; 
			 close(tube2[1]) ; 
			 a = 0; 
 			 deb = clock() ;
			 while (1)
			 {
				a += 1 ; 
				write(tube1[1] , &a, L) ;
			 	read(tube2[0] , &a, L) ; 
				if (a == (nb_ping_pong*2))
				{
					fin = clock()  ; 
					kill(pid, SIGINT) ; 
				//	printf("%d : a = %d" , getpid() , a) ; 
					break;
				}
				//printf("%d : a = %d\n" , getpid() , a) ; 
		         }
			 *time = ((double)(fin - deb))/CLOCKS_PER_SEC ;
			 d = (((nb_ping_pong)*2*4*8)*(pow(10,-6))/(*time)) ;
			 if (d > 255)
			 {	
		  	 	sprintf(unit, "Mo/s") ; 
	 	 		return (((nb_ping_pong)*2*4)*(pow(10,-6))/(*time)) ;
				break ; 
	 		 }	 
			 else
	 		 {
		 	 	sprintf(unit, "Mb/s") ;  
		 		return d ; 
				break ; 
			 }		 
        }
	exit(0) ; 
}   

int main() 
{
	
	char *unit = malloc(5*sizeof(char)) ; 
	float res ;
	int nb_ping_pong ; 
	int i ; 
	int iter ; 
	double *clock = malloc(sizeof(double)) ; 	
	printf("Veuillez choisir un nombre de ping-pong : ") ;
	scanf("%d" , &nb_ping_pong) ; 
	printf("\n") ;
	printf("Veuiller choisir un nombre d'itérations de test : ") ;
	scanf("%d" , &iter) ; 
	printf("\n") ; 
	for(i = 1 ; i < (iter+1) ; i++)
	{
	res = calcul_débit_tube(nb_ping_pong, unit, clock) ;  
	printf("\nTest n°%d :\n" , i) ;
	printf("Débit : %f %s\n", res, unit) ;
	printf("Durée du transfert : %lf seconde(s)\n" , *clock) ;
	}
	printf("\n") ; 
	return 0; 
}












