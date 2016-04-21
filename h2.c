/*
    Homework n.2

    Estendere l'esempio 'move.c' visto a lezione per supportare i 2 casi speciali:
    - spostamento cross-filesystem: individuato tale caso, il file deve essere
      spostato utilizzando la strategia "copia & cancella";
    - spostamento di un link simbolico: individuato tale caso, il link simbolico
      deve essere ricreato a destinazione con lo stesso contenuto (ovvero il percorso
      che denota l'oggetto referenziato); notate come tale spostamento potrebbe
      rendere il nuovo link simbolico non effettivamente valido.

    La sintassi da supportare e' la seguente:
     $ homework-2 <pathname sorgente> <pathname destinazione>
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <libgen.h>
#include <unistd.h>
#include <stdbool.h>

#define BUFSIZE 4096
#define MODE 0660

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "utilizzo: %s <file-esistente> <nuovo-nome-e-o-posizione>\n", argv[0]);
        exit(1);
    }
	else{
		struct stat src_stat, dst_stat;
		int s, d, size;
		char buffer[BUFSIZE];
		bool esito=false;
		if(lstat(argv[1], &src_stat)==-1){
			perror(argv[1]);
			exit(1);
		}
		char* dst_d;
		char tmp[255];
		strcpy(tmp,argv[2]);
		dst_d=dirname(tmp);
		if (stat(dst_d, &dst_stat) == -1) {
		    perror(dst_d);
		    exit(1);
        	}
		if((src_stat.st_mode & S_IFMT) == S_IFREG){
			if(src_stat.st_dev==dst_stat.st_dev){
				/* crea un hard-link a partire dal file esistente */
				printf(" stesso fs\n");
				if (link(argv[1], argv[2]) == -1) {
					perror(argv[2]);
					exit(1);
				}
				esito=true;
			}
			else{
				printf(" diverso FS\n");
				if ((s=open(argv[1], O_RDONLY))== -1)
				{
					perror(argv[1]);
					exit(1);
				}
				if ((d=open(argv[2], O_RDWR|O_TRUNC|O_CREAT, (src_stat.st_mode & 0777)))==-1){
					perror(argv[2]);
					exit(1);
				}
				do{
					if ((size=read(s, buffer, BUFSIZE))==-1){
						perror(argv[1]);
						exit(1);
					}
					
					if ((write(d, buffer, size))==-1){
						perror(argv[2]);
						exit(1);
					}
				} while (size==BUFSIZE);
				close(s);
				close(d);
				esito=true;
			}
		}
		else if((src_stat.st_mode & S_IFMT) == S_IFLNK){
			printf(" link\n");
			if((size=readlink(argv[1], buffer, BUFSIZE))==-1){
				perror(argv[1]);
				exit(1);
			}
			buffer[size]='\0'; //EOF
			if(symlink(buffer, argv[2])==-1){
				perror(argv[2]);
				exit(1);
			}
			esito=true;
		}
		
		/* rimuove il vecchio riferimento al file */
		if(esito){
			printf(" elimino\n");
			if (unlink(argv[1]) == -1) {
				perror(argv[1]);
				exit(1);
			}
		}
		else {printf("non ho fatto niente :(\n");}
    }
}
