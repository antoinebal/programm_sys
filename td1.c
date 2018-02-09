#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>



void* fonction1 (void* arg) {
int no;
 for (no=0; no<*arg; no++) {
printf("et mon courroux");
sleep(s);
}

}




int main () {

pthread tid;
int* m;
int* n;
int no;
scanf("%d", &m);
scanf("%d", &n);

pthread_create(tid, NULL, fonction1, m);

for (no=0; no<*m; no++) {
printf("coucou");
sleep(s);
}

return 0;
}

