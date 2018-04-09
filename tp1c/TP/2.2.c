
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
é
int main()
{

	int key = 32 ;
	int semaphore ; 
	struct sembuf sop_P , sop_V ;  
	semaphore = semget((key_t)key , 1 , 0666) ; 
	if (semaphore == -1) 
	{
		semaphore = semget(key, 1, 0666 | IPC_CREAT) ; 
		semctl(semaphore, 0, SETVAL, 1) ;
	}
	while (1) 
	{
		sop_P.sem_num = 0 ;
		sop_P.sem_flg = 0 ; 
		sop_P.sem_op = -1 ; 
		semop(semaphore, &sop_P , 1) ; 
		printf("Veuillez saisir une chaîne svp :\n") ; 
		char chaine[20] ; 
		scanf("%s" , chaine) ;
		sop_V.sem_num = 0 ; 
		sop_V.sem_flg = 0 ; 
		sop_V.sem_op = 1 ; 
		semop(semaphore, &sop_V, 1) ; 
	}
	return 0; 
} 
