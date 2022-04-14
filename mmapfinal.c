#include <sys/mman.h>   /* mmap()  */
#include <stdlib.h>     /* exit() */
#include <unistd.h>     /* fork() and getpid() */
#include <stdio.h>      /* printf() */
#include <sys/wait.h>   /*wait*/
#include <errno.h>	    /* errno */

#define MSGSIZE 2
int total;
int main(int argc, char **argv) {
        int produsen;//jumlah produsen
	printf("Input Jumlah Produsen : ");
	scanf("%d", &produsen);
	int konsumen;//jumlah konsumen
	printf("Input Jumlah Konsumen : ");
	scanf("%d", &konsumen);
        
        total =0;
        int pid;
        int i;
        int size = MSGSIZE * sizeof(int);
//buat memory map
        void *addr = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        printf("Mapped at %p\n", addr);
        int *shared = addr;
        switch (pid = fork()) {
        case 0:
                 for (i = 0; i < produsen; i++){
			fork();
        }
		printf("Start Produsen %d\n", getpid());
        /* tulis data ke pipe */
        srand(getpid());
		for (i = 0; i<MSGSIZE; i++){
			shared[i] = rand()%10;
			printf("Parent --> Produsen Menulis %d\n", shared[i]);
		}
                break;
        default:        
                for (i = 0; i < konsumen; i++){
            fork();
        }
                int pidWait,status;
                /* wait sampai child selesai */
                while (pidWait = wait(&status)) {   
                    if (pidWait == pid)  /* child sukses selesai*/
                        //cetak isi memory map
                        
      
        printf("Start Konsumen %d\n", getpid());
        for(i=0; i<MSGSIZE; i++){
            printf("Child --> Konsumen Membaca %d\n", shared[i]);
             
            total += shared[i];
            printf("total = %d\n", total);
            ;
        }
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
