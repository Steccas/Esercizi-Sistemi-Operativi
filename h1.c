/*
    Homework n.1

    Scrivere un programma in linguaggio C che permetta di copiare un numero
    arbitrario di file regolari su una directory di destinazione preesistente.

    Il programma dovra' accettare una sintassi del tipo:
     $ homework-1 file1.txt path/file2.txt "nome con spazi.pdf" directory-destinazione
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define BUFSIZE 4096
#define MODE 0660

int main(int argc, char *argv[])
{
	if (argc > 2)
	{
		int i;
	    	char *path=argv[argc-1];
	    	strcat(path, "/");
    		for (i=1; i < argc -1;  i++)
    		{
    			int sd, dd, size, result;
    			char buffer[BUFSIZE];
    			char n[255];
    			strcpy(n, path);
    			printf("i: %i \n", i);
    			//sorgente
    			if ((sd = open(argv[i], O_RDONLY)) == -1) {
	       			perror(argv[i]);
				exit(1);
    			}
    			//destinazione
    			printf("path:");
    			printf(path);
    			printf("\n");
    			printf("argv:");
    			printf(argv[i]);
    			printf("\n");
    			strcat(n, argv[i]);
    			printf("n:");
    			printf(n);
    			printf("\n");
    			printf("Eseguo...\n");
    			if ((dd = open(n, O_RDWR|O_TRUNC|O_CREAT, MODE)) == -1) {
        			perror(n);
        			exit(1);
    			}
    			
    			// copia i dati dalla sorgente alla destinazione
   				do {
       			 	// legge fino ad un massimo di BUFSIZE byte dalla sorgente
        			if ((size = read(sd, buffer, BUFSIZE)) == -1) {
		    			perror(argv[i]);
			    		exit(1);
        			}
        			// scrive i byte letti
        			if ((result = write(dd, buffer, size)) == -1) {
		    			perror(argv[argc-1]);
		    			exit(1);
        			}
    			} while (size == BUFSIZE);

	 			// chiude i file prima di uscire
				close(sd);
				close(dd);
    		}
	}
	else
	{
		printf("utilizzo: %s [<sorgente>...] <directory destinazione>\n", argv[0]);
        	exit(1);
	}
}
