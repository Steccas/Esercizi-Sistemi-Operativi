/*    Homework n.4

    Estendere l'esercizio 'homework n.1' affinche' operi correttamente
    anche nel caso in cui tra le sorgenti e' indicata una directory, copiandone
    il contenuto ricorsivamente. Eventuali link simbolici incontrati dovranno
    essere replicati come tali (dovrà essere creato un link e si dovranno
    preservare tutti permessi di accesso originali dei file e directory).

    Una ipotetica invocazione potrebbe essere la seguente:
     $ homework-4 directory-di-esempio file-semplice.txt path/altra-dir/ "nome con spazi.pdf" directory-destinazione
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
#include <errno.h>
#include <dirent.h>

#define BUFSIZE 4096

void copia(char *sorg, char *path)
{
	struct stat src_stat, dst_stat;
	int s, d, size;
	char buffer[BUFSIZE];
	bool esito=false;
	if(lstat(sorg, &src_stat)==-1){
		perror(sorg);
		exit(1);
	}
	char tp[255];
	strcpy(tp, path);
	char *dst_d=dirname(tp);
	if (stat(dst_d, &dst_stat) == -1) {
		    perror(dst_d);
		    exit(1);
    }
   	
	if((src_stat.st_mode & S_IFMT) == S_IFREG){
		if(src_stat.st_dev==dst_stat.st_dev){
			/* crea un hard-link a partire dal file esistente */
			printf("stesso fs\n");
			if (link(sorg, path) == -1) {
				perror(path);
				exit(1);
			}
			esito=true;
		}
		else{
			printf("diverso FS\n");
			if ((s=open(sorg, O_RDONLY))== -1)
			{
				perror(sorg);
				exit(1);
			}
			if ((d=open(path, O_RDWR|O_TRUNC|O_CREAT, (src_stat.st_mode & 0777)))==-1){
				perror(path);
				exit(1);
			}
			do{
				if ((size=read(s, buffer, BUFSIZE))==-1){
					perror(sorg);
					exit(1);
				}
					
				if ((write(d, buffer, size))==-1){
					perror(path);
					exit(1);
				}
			} while (size==BUFSIZE);
			close(s);
			close(d);
			esito=true;
		}
	}
	
	if((src_stat.st_mode & S_IFMT) == S_IFLNK){
		printf("link\n");
		if((size=readlink(sorg, buffer, BUFSIZE))==-1){
			perror(sorg);
			exit(1);
		}
		buffer[size]='\0'; //EOF
		if(symlink(buffer, path)==-1){
			perror(path);
			exit(1);
		}
		esito=true;
	}
	
	//if ((src_stat.st_mode & S_IFMT) == S_IFDIR)
	if (S_ISDIR(src_stat.st_mode))
	{
		printf("Directory\n");
		if (mkdir(path, dst_stat.st_mode & 0777) == -1) {
            perror(path);
            if (errno != EEXIST) exit(1);
        }
        
        DIR *dir;
        if ((dir=opendir(sorg))==NULL)
        {
        	perror(path);
        	exit(1);
        }
        
        struct dirent *entry;
        while ((entry=readdir(dir)) != NULL)
        {
        	if (strcmp(entry->d_name,".") == 0  || strcmp(entry->d_name,"..") == 0 )
                continue;
            printf("ciclo\n");
            //<directory destinazione>/<sorgente>
            char tmp[255];
            strcpy(tmp, sorg);
            strcat(tmp, "/");
            strcat(tmp, entry->d_name);
            char tmp2[255];
            strcpy(tmp2, path);
            strcat(tmp2, "/");
            strcat(tmp2, entry->d_name);
            copia(tmp, tmp2);
        }
        esito=true;
	}
	
	if(esito){
		printf("OK\n");
	}
	else {printf("non ho fatto niente :(\n");}
	
}

int main(int argc, char* argv[])
{
	if (argc > 2)
	{
		int i;
	    char *p=argv[argc-1];
	   	strcat(p, "/");
    	for (i=1; i < argc -1;  i++)
   		{
   			char d[255];
    		strcpy(d, p);
    		printf("i: %i \n", i);
    		//sorgente
    		//destinazione
    		printf("path:");
    		printf(p);
   			printf("\n");
   			printf("sorg:");
   			printf(argv[i]);
   			printf("\n");
    		strcat(d, argv[i]);
    		printf("dest:");
    		printf(d);
   			printf("\n");
   			printf("Eseguo...\n");
   			copia(argv[i], d);
    	}
	}
	else
	{
		printf("utilizzo: %s [<sorgente>...] <directory destinazione>\n", argv[0]);
        	exit(1);
	}
}
