#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <pthread.h>
#define taille_matrice 1000

//à passer en arg à la fonction du thread
typedef struct {
  int indice_premiere_ligne_;
  int indice_derniere_ligne_;
  int* D_; //
  int* V_;
  int* V_New_;
  int id_;
} S_Arg;

//utile pour simplifier la création du thread
typedef void *(*fct_ptr_type)(void *);

/* variable pour indiquer si un thread est le
   dernier à finir ou non.
   0 <= indicateur < nombre de threads
*/
static int indicateur;

//mutex pour protéger l'indicateur
static pthread_mutex_t mutexIndicateur = PTHREAD_MUTEX_INITIALIZER;

//cond pour attendre le dernier thread
static pthread_cond_t condAttenteDernier = PTHREAD_COND_INITIALIZER;

//défini par l'utilisateur
static int nbreIterations;

//défini par l'utilisateur
static int nbreThreads;


void permuteAdresses(int* adr1, int* adr2) {
  int* aux = adr1;
  adr1 = adr2;
  adr2 = aux;
}


//fonction du thread
//les threads crées traitent les premiers blocs de matrice
//(sans le reste)
void winterfell(S_Arg *arg) {

  printf("Thread n° %d crée. \n", (*arg).id_);
  
  for (int i = 0 ; i < nbreIterations ; i++) {

    //MULTIPLICATION
    for (int NO = (*arg).indice_premiere_ligne_ ; NO <= (*arg).indice_derniere_ligne_ ; NO++) {
      for (int NB = 0 ; NB < taille_matrice ; NB++) {
	(*arg).V_New_[NO] = (*arg).V_New_[NO] + (*arg).D_[NO*taille_matrice + NB]*(*arg).V_[NB];
      }
    }

    //l'opération est finie
    if (indicateur < nbreThreads) {
      //pas le dernier à finir, on incrémente seulement l'indicateur
      pthread_mutex_lock(&mutexIndicateur);
      indicateur++;
      printf("Le thread n° %d a fini en %d ème \n", (*arg).id_, indicateur);
      pthread_mutex_unlock(&mutexIndicateur);
     
    } else {
      /*ce thread est le dernier à finir, il doit remettre
	l'indicateur à 0 et prévenir les autres threads*/
      //également, on le fait permutter les adresses V_New_ et V_
      pthread_mutex_lock(&mutexIndicateur);
      indicateur = 0;
      pthread_mutex_unlock(&mutexIndicateur);

      permuteAdresses((*arg).V_ , (*arg).V_New_);

      pthread_cond_broadcast(&condAttenteDernier);
    }
      
  }
}


  //le main s'occupera de la dernière part de ligne + du reste récup avec le mod
  int main () {
    //CREATION DE LA MATRICE D
    int* D=(int*)malloc(taille_matrice*taille_matrice*sizeof(int));

    //on la remplit
    int i=0;
    int j=0;
    for(i=0 ; i < taille_matrice ; i++) {
      for(j=0 ; j < taille_matrice ; j++) {
	D[i*taille_matrice + j]  = rand()%10+1; //pour que 1 < D[i][j] < 10
      }
    }
    
    
    //CREATION DU VECTEUR V[0]
    int V[taille_matrice];
    //on la remplit
    for(j=0 ; j < taille_matrice ; j++) {
      V[j] = rand()%10+1; //pour que 1 < V[j] < 10
    }
    
    //ALLOCATION MEMOIRE POUR V_NEW
    //le mieux est de le faire ici, et on switch les adresses de V_ et V_New_ au lieu
    //de copier les tableaux
    int V_New[taille_matrice];

    //RECUPERATION DES PARAMETRES
    printf("Nombre de threads : ");
    scanf("%d", &nbreThreads);

    printf("Nombre d'itérations : ");
    scanf("%d", &nbreIterations);

    //CREATION DU TABLEAU DE TID
    pthread_t tidTab[nbreThreads-1];

    //CREATION DES THREADS
    int ligneCourante = 0;
    int lignesParThread = taille_matrice/nbreThreads;
    for (int i = 0 ; i < nbreThreads-1 ; i++) {
      S_Arg sCourant;
      sCourant.indice_premiere_ligne_ = ligneCourante;
      sCourant.indice_derniere_ligne_ = ligneCourante + lignesParThread;
      sCourant.D_ = D;
      sCourant.V_ = V;
      sCourant.V_New_ = V_New;
      sCourant.id_ = i+1;

      ligneCourante = ligneCourante + lignesParThread + 1;
    
      pthread_create(&tidTab[i], NULL, (fct_ptr_type)winterfell, &sCourant);
    }

    int premiere_ligne_main = ligneCourante;
    int derniere_ligne_main = taille_matrice -1;

    for (int i = 0 ; i < nbreIterations ; i++) {

      //MULTIPLICATION
      for (int NO = premiere_ligne_main ; NO <= derniere_ligne_main ; NO++) {
	for (int NB = 0 ; NB < taille_matrice ; NB++) {
	  V_New[NO] = V_New[NO] + D[NO*taille_matrice + NB]*V[NB];
	}
      }

      
      //l'opération est finie
      if (indicateur < nbreThreads) {
	//pas le dernier à finir, on incrémente seulement l'indicateur
	pthread_mutex_lock(&mutexIndicateur);
	indicateur++;
	pthread_mutex_unlock(&mutexIndicateur);
      } else {
	/*ce thread est le dernier à finir, il doit remettre
	  l'indicateur à 0 et prévenir les autres threads*/
	//également, on le fait permutter les adresses V_New et V
	pthread_mutex_lock(&mutexIndicateur);
	indicateur = 0;
	printf("Le thread main a fini en %d ème \n", indicateur);
	pthread_mutex_unlock(&mutexIndicateur);

	permuteAdresses(V , V_New);

	pthread_cond_broadcast(&condAttenteDernier);
      }

    }

  }
