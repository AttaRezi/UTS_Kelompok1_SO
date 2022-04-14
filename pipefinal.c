#include <stdlib.h>     /* exit() */
#include <unistd.h>     /* fork() and getpid() */
#include <stdio.h>     /* fork() and getpid() */
#include <string.h>     /* fork() and getpid() */
#define MSGSIZE 2
int main(int argc, char **argv){
	int produsen;//jumlah produsen
	printf("Input Jumlah Produsen : ");
	scanf("%d", &produsen);
	int konsumen;//jumlah konsumen
	printf("Input Jumlah Konsumen : ");
	scanf("%d", &konsumen);
	
   int pipe1[MSGSIZE];
   int returnstatus;
   int pid;
    int total = 0;
   int readmsg[MSGSIZE];
   int i;
   
   returnstatus = pipe(pipe1);
   if (returnstatus == -1) {
      printf("Unable to create pipe\n");
      return 1;
   }
   pid = fork();
   
   // Child process
   if (pid == 0) {
	   for (i = 0; i < konsumen; i++){
            fork();
        }
      
        printf("Start Konsumen %d\n", getpid());
        read(pipe1[0], readmsg, sizeof(readmsg));
        for(i=0; i<MSGSIZE; i++){
            printf("Child --> Konsumen Membaca %d\n", readmsg[i]);
             
            total += readmsg[i];
            printf("total = %d\n", total);
            ;
        }
        
   } else { //Parent process
   for (i = 0; i < produsen; i++){
			fork();
        }
		printf("Start Produsen %d\n", getpid());
        /* tulis data ke pipe */
        srand(getpid());
       int temp1[MSGSIZE];
		for (i = 0; i<MSGSIZE; i++){
			temp1[i] = rand()%10;
			printf("Parent --> Produsen Menulis %d\n", temp1[i]);
		}
		write(pipe1[1], temp1, sizeof(temp1));
   }
   return 0;
}