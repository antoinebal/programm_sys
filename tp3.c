#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#define taille_matrice 1000

struct S_Arg{
  int indice_premier_ligne;
  int indice_derniere_ligne;
  

//fonction du thread
void courroux(void* arg) {


}



int main (void) {
  //CREATION DE LA MATRICE D
  int* D=(int*)malloc(taille_matrice*taille_matrice*sizeof(int));

  //on la remplit
  int i=0;
  int j=0;
  for(i=0 ; i < taille_matrice ; i++) {
    for(j=0 ; j < taille_matrice ; j++) {
      D[i* = rand()%10+1; //pour que 1 < D[i][j] < 10
    }
  }

  //CREATION DU VECTEUR V[0]
  int V[taille_matrice];
  //on la remplit
  for(j=0 ; j < taille_matrice ; j++) {
    V[j] = rand()%10+1; //pour que 1 < V[j] < 10
  }

  //RECUPERATION DES ARGUMENTS
  int nbreThreads;
  int nbreIterations;

  printf("Nombre de threads : ");
  scanf("%d", nbreThreads);

  printf("Nombre d'itérations : ");
  scanf("%d", nbreIterations);

  //CREATION DU TABLEAU DE TID
  pthread_t tidTab[nbreThreads];

  
  

  



}
