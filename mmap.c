#include <sys/mman.h>   /* mmap()  */
#include <stdlib.h>     /* exit() */
#include <unistd.h>     /* fork() and getpid() */
#include <stdio.h>      /* printf() */
#include <sys/wait.h>   /*wait*/
#include <errno.h>	    /* errno */
#include <stdio.h>
#include <string.h>

#define MSGSIZE 3

int main(int argc, char **argv) {
        int pid;
        int i;
        int size = MSGSIZE * sizeof(int);
        //buat memory map
        void *addr1 = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        void *addr2 = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        printf("Mapped at %p\n", addr1);
        int *shared1 = addr1;
        char *shared2 = addr2;
        int temp1[100];
        int temp2 = 0; //temp2 untuk menampung nilai penjumlahan dari keseluruhan nilai temp1
        char temp3[100];
        for(i=0; i<MSGSIZE; i++){
            scanf("%d", &temp1[i]);
            temp2 = temp2 + temp1[i];
        }
        sprintf(temp3,"%d", temp2);
        switch (pid = fork()) {
        case 0:         /*  proses anak */

            //ketika proses parent dijalankan, disaat yg sama proses child juga berjalan
                printf("Proses anak \n");
                //isi data di memory map
                for(i=0; i<MSGSIZE; i++){
                    shared1[i] = temp1[i];
                   
                }
                strcpy(shared2, temp3);
                
                break;
        default:        /*  ortu */
                printf("Proses ortu\n");
                int pidWait,status;
                /* wait sampai child selesai */
                while (pidWait = wait(&status)) {   
                    if (pidWait == pid)  /* child sukses selesai*/
                        //cetak isi memory map
                        for (i = 0; i<MSGSIZE; i++) {
                          printf("ini parent, proses child menulis: %d \n", shared1[i]);
                        }
                        printf("ini parent, proses child menulis: %s \n", shared2);
                        break; //keluar dari loop wait
                    if ((pidWait == -1) && (errno != EINTR)) {
                        /* ada error*/
                        perror("waitpid");
                        exit(1);
                    }
                }
                break;
        case -1:        /* error */
                perror("fork");
                exit(1);
        }
        exit(0);
}
