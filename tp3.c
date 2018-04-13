#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <pthread.h>
#define taille_matrice 10

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

//pour savoir si tous les threads ont été créés
//static int onPeutCommencer;


void permuteAdresses(int* adr1, int* adr2) {
  int* aux = adr1;
  adr1 = adr2;
  adr2 = aux;
}

//afficher matrice 2D
void afficherMatrice(int* mat) {
    for (int NO = 0 ; NO < taille_matrice ; NO++) {
	for (int NB = 0 ; NB < taille_matrice ; NB++) {
	  printf("%d", mat[NO*taille_matrice + NB]);
	}
	printf("\n");
      }
}


void afficherVecteur(int* vec) {
  for (int NO = 0 ; NO < taille_matrice ; NO++) {
    printf("%d \n", vec[NO]);
  }
}


//fonction du thread
//les threads crées traitent les premiers blocs de matrice
//(sans le reste)
void winterfell(S_Arg *arg) {

  //si le thread a fini dernier, il n'a pas besoin d'attendre la cond
  int finiDernier=0;
  printf("Thread n° %d crée. \n", (*arg).id_);

  if ((*arg).id_==nbreThreads-1) { //cela veut dire que tous les threads ont été crées
    //(celui-ci étant le dernier crée par le main)
    //on émet le signal quand tous les threads sont crées aussi
     pthread_cond_broadcast(&condAttenteDernier);
     printf("Thread n° %d a fait le broadcast. \n", (*arg).id_);
  }
  
  for (int i = 0 ; i < nbreIterations ; i++) {
    if (finiDernier ==0) {
    if ((i!=0)|((*arg).id_!=nbreThreads-1)){ //pour sa première it, le dernier thread crée 
      printf("Thread n° %d attend. \n", (*arg).id_);//ne doit pas attendre
      pthread_cond_wait(&condAttenteDernier, &mutexIndicateur);
      pthread_mutex_unlock(&mutexIndicateur);
      printf("Thread %d est passé \n", (*arg).id_);
    }
    }
    finiDernier = 1;

    //MULTIPLICATION
    for (int NO = (*arg).indice_premiere_ligne_ ; NO <= (*arg).indice_derniere_ligne_ ; NO++) {
      for (int NB = 0 ; NB < taille_matrice ; NB++) {
	(*arg).V_New_[NO] = (*arg).V_New_[NO] + (*arg).D_[NO*taille_matrice + NB]*(*arg).V_[NB];
      }
    }

    //l'opération est finie
    printf("Thread %d valeur indicateur : %d \n", (*arg).id_, indicateur);
    if (indicateur < nbreThreads-1) {
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
      printf("Le thread n° %d a fini en dernier \n", (*arg).id_);
      printf("Iteration n° %d finie \n", i);
      pthread_mutex_unlock(&mutexIndicateur);

      permuteAdresses((*arg).V_ , (*arg).V_New_);

      pthread_cond_broadcast(&condAttenteDernier);
      finiDernier = 1;
    }
      
  }
}


  //le main s'occupera de la dernière part de ligne + du reste récup avec le mod
  int main () {
    //si le thread a fini dernier, il n'a pas besoin d'attendre la cond
    int finiDernier=0;
  
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

    printf("D : \n");
    afficherMatrice(D);
    
    
    //CREATION DU VECTEUR V[0]
    int V[taille_matrice];
    //on la remplit
    for(j=0 ; j < taille_matrice ; j++) {
      V[j] = rand()%10+1; //pour que 1 < V[j] < 10
    }

    printf("V[0] : \n");
    afficherVecteur(V);
    
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

    //CREATION DU TABLEAU DE S_ARG
    /* en fait pb car on filait en argument un pointeur vers
       le même S_Arg, du coup premiere et derniere lignes, id
       etc étaient les mêmes.*/
    S_Arg tabSArg[nbreThreads-1];
    int ligneCourante = 0;
    int lignesParThread = taille_matrice/nbreThreads;
    for (int i = 0 ; i < nbreThreads-1 ; i++) {
      tabSArg[i].indice_premiere_ligne_ = ligneCourante;
      tabSArg[i].indice_derniere_ligne_ = ligneCourante + lignesParThread;
      tabSArg[i].D_ = D;
      tabSArg[i].V_ = V;
      tabSArg[i].V_New_ = V_New;
      tabSArg[i].id_ = i+1;

      ligneCourante = ligneCourante + lignesParThread + 1;
    }
    

    //CREATION DES THREADS
    for (int i = 0 ; i < nbreThreads-1 ; i++) {    
      pthread_create(&tidTab[i], NULL, (fct_ptr_type)winterfell, &tabSArg[i]);
    }
 

    int premiere_ligne_main = ligneCourante;
    int derniere_ligne_main = taille_matrice -1;

    for (int i = 0 ; i < nbreIterations ; i++) {
      printf("%d", i);
      if (finiDernier == 0) {
      printf("Le thread main attend \n");
      pthread_cond_wait(&condAttenteDernier, &mutexIndicateur);
      pthread_mutex_unlock(&mutexIndicateur);
      printf("Thread main est passé \n");
      }
      finiDernier = 0;
      

      //MULTIPLICATION
      for (int NO = premiere_ligne_main ; NO <= derniere_ligne_main ; NO++) {
	for (int NB = 0 ; NB < taille_matrice ; NB++) {
	  V_New[NO] = V_New[NO] + D[NO*taille_matrice + NB]*V[NB];
	}
      }

      
      //l'opération est finie
      if (indicateur < nbreThreads-1) {
	//pas le dernier à finir, on incrémente seulement l'indicateur
	pthread_mutex_lock(&mutexIndicateur);
	indicateur++;
	printf("Le thread main a fini en %d ème \n", indicateur);
	pthread_mutex_unlock(&mutexIndicateur);
      } else {
	/*ce thread est le dernier à finir, il doit remettre
	  l'indicateur à 0 et prévenir les autres threads*/
	//également, on le fait permutter les adresses V_New et V
	pthread_mutex_lock(&mutexIndicateur);
	indicateur = 0;
	printf("Le thread main a fini en dernier \n");
	printf("Iteration n° %d finie \n", i);
	pthread_mutex_unlock(&mutexIndicateur);
	
	permuteAdresses(V , V_New);

	pthread_cond_broadcast(&condAttenteDernier);
	finiDernier = 1;
      }

    }

}
