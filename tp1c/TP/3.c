#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void afficher_vecteur(int *vecteur, int taille)
{
	int i ; 
	printf("Vecteur :\n") ;
	for(i=0; i < taille; i++)
	{
		printf("%d\n" , vecteur[i]) ; 
	}
}

void afficher_matrice(int *matrice[3])
{
	int i , j ; 
	printf("Matrice A :") ; 
	for(i = 0; i < 3; i++)
	{
		printf("\n") ; 
	for(j = 0; j < 3; j++)
	{
		printf("%d   ", matrice[i][j]) ;
	}
	}
	printf("\n") ;
}

/*void thread_1(void *arg)
{
	int i ; 
	if (params->meth == 1)
	{
		for(i = 0, i < params->D, i++
	}

}

void thread_2(void *arg)
{

}

void thread_3(void *arg)
{

}
*/

int main(int argc, const char *argv[])
{
	/*Gestion d'entrée des arguments*/
	if ((argc-1) != 3)
	{
		printf("Il n'y a pas 3 arguments\n") ; 
		printf("Nombre d'arguments : %d\n" , argc) ;
		exit(1) ; 
	}
	if (!((atoi(argv[1]) == 1) ||  ((atoi(argv[1]) == 2))))	
	{
		printf("Saisie méthode incorrecte, veuillez choisir entre 1 (ligne) et 2 (colonne)\n") ;
		printf("%d\n" , atoi(argv[1])) ;  
		exit(1) ; 
	}
	if (!((atoi(argv[2]) >= 1) &&  ((atoi(argv[2]) <= 4))))
	{
		printf("Nombre de threads choisi incorrect, veuillez choisir entre 1 et 4\n") ; 
		printf("%d\n" , atoi(argv[2])) ;  
		exit(1) ; 
	}
	
	/*Création et affichages du vecteur et de la matrice de calcul*/
	int **A = {{1,2,3}, {4,5,6}, {7,8,9}} ; 
	int *V = {3, 5, 2} ;
	/*int i , j ; 
	printf("Vecteur V :\n") ;
	for(i=0; i < 3; i++)
	{
		printf("%d\n" , V[i]) ; 
	}
	printf("Matrice A :") ; 
	for(i = 0; i < 3; i++)
	{
		printf("\n") ; 
	for(j = 0; j < 3; j++)
	{
		printf("%d   ", A[i][j]) ;
	}
	}
	printf("\n") ;*/

	afficher_vecteur(V , 3) ; 
	afficher_matrice(A) ; 

	/*Création d'une structure pour stocker les paramètres*/
	struct PARAMETRES
	{
		int meth ; 
		int D ; 
		int *vecteur ;
		int **matrice ; 
	};
	struct PARAMETRES *params = malloc(sizeof(struct PARAMETRES)) ; 
	params->meth = atoi(argv[1]) ;
	params->D = 3 ; 
	params->vecteur = V ; 
	params->matrice = A ; 
	
	/*Création des threads de calcul
	pthread_t thread1 ; 
	pthread_t thread2 ;
	pthread_t thread3 ; 
	pthread_create(&thread1, NULL, thread_1, NULL) ; 
	pthread_create(&thread2, NULL, thread_2, NULL) ; 
	pthread_create(&thread3, NULL, thread_3, NULL) ;*/ 
}





