#include <stdlib.h>     /* exit() */
#include <unistd.h>     /* fork() and getpid() */
#include <stdio.h>     /* fork() and getpid() */
#include <string.h>     /* fork() and getpid() */

#define MSGSIZE 3

int main(int argc, char **argv) {
   int pipe1[2], pipe2[2];
   int i;
   
   int temp1[MSGSIZE];
   int temp2 = 0; //temp2 untuk menyimpan data penjumlahan semua nilai dari temp1
    for (i = 0; i < MSGSIZE; i++) {
           printf("INPUT DATA ");
           scanf("%d", &temp1[i]);
           temp2 += temp1[i];
       }
   //proses pengubahan tipe data dari temp2 menjadi string agar dapat diproses oleh write dan read
   char temp3[100];
   sprintf(temp3,"%d", temp2);
   
   int readmessage1[MSGSIZE];
   char readmessage2[100];
   
   //buat pipe
   int returnstatus1, returnstatus2;
   returnstatus1 = pipe(pipe1);
       
   if (returnstatus1 == -1) {
      printf("Unable to create pipe 1 \n");
      return 1;
   }
   returnstatus2 = pipe(pipe2);
   
   if (returnstatus2 == -1) {
      printf("Unable to create pipe 2 \n");
      return 1;
   }

   int pid;

   pid = fork();
   
   if (pid != 0){ // Parent process {
      close(pipe1[0]); // tutup proses read dari pipe1
      close(pipe2[1]); // tutup proses write dari pipe2
     
       for (i = 0; i<MSGSIZE; i++) {
           printf("In Parent: Writing to pipe 1 – Message is %d\n", temp1[i]);
       }
      write(pipe1[1], temp1, sizeof(temp1)); 
      //printf("%d\n", pid);
      //langsung pergi ke read pipe1 di child
      
      read(pipe2[0], readmessage2, sizeof(readmessage2));
      //printf("%d\n", pid);
      printf("In Parent: Reading from pipe 2 – Message is %s\n", readmessage2);
     
   } else { //child process
      close(pipe1[1]); // tutup proses write dari pipe1
      close(pipe2[0]); // tutup proses read dari pipe2
      
      read(pipe1[0], readmessage1, sizeof(readmessage1));
      for (i = 0; i<MSGSIZE; i++) {
           printf("In Child: Reading from pipe 1 – Message is %d\n", readmessage1[i]);
      }
     
      printf("In Child: Writing to pipe 2 – Message is %s\n", temp3);
      write(pipe2[1], temp3, sizeof(temp3)); //kembali ke proses parent, tepatnya ke proses read pipe 2
    
   }
   return 0;
}
