/*
    Homework n.5

    Estendere l'esempio 'nanoshell.c' ad una shell piu' realistica in cui
    si possono:
    - passare argomenti al comando invocato (per semplicita', assumiamo
    che questi non contengano spazi);
    - usare la redirezione dei canali di input/output/error;
    - mandare un comando in esecuzione in background (con la '&' finale).

    Esempi di invocazione che la shell dovrebbe supportare sono:
    $ cal 3 2015
    $ cp /etc/passwd /etc/hosts /tmp
    $ cat </dev/passwd >/tmp/passwd
    $ cat filechenonesiste 2>/dev/null
    $ ls -R /etc/ &
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>

#define LEN_BUFFER 2048
#define DELIM   " "
#define MAX_ARGS 30

#define STDIN 0
#define STDOUT 1
#define STDERR 2

int main(int argc, char *argv[])
{
	
	int pid, fd;
	printf("Benvenuto in SteccaShell! Digita 'quit' o 'exit' per uscire.\n");
	while(1)
	{
		char buffer[LEN_BUFFER];
		bool bkg=false;
		char *comandi[MAX_ARGS];
		int i=0;
		int numC;
		int len;
		char *path_stdout, *path_stdin, *path_stderr;
		char *args[MAX_ARGS];
		int c= MAX_ARGS + 1;
		bool caso=false;
		int k=0;
		int j;
		printf("$ ");
		fgets(buffer, LEN_BUFFER, stdin);
		/*char *comtoken;
		comtoken=strtok(buffer, DELIM);*/
		comandi[i]=strtok(buffer, DELIM);
		while(comandi[i] != NULL)
		{
			comandi[++i] = strtok(NULL, DELIM);
		}
		i--;
		numC=i;
		i=0;
		len = strlen(comandi[numC]);
		if ( comandi[numC][len-1] == '\n') comandi[numC][len-1] = '\0';
		/*printf(comandi[numC]);
		printf("\n");
		printf(comandi[0]);
		printf("\n");*/
		if ((strcmp(comandi[numC], "exit") == 0) || (strcmp(comandi[numC],"quit") == 0)) {printf("Arrivederci!\n"); break;}
		if (strcmp(comandi[numC],"&")) bkg=true;
		//if (comtoken[0] != NULL) printf(comtoken[0]);
		path_stdout = path_stdin = path_stderr = NULL;
		for (i=0; i <= numC; i++)
		{
			caso=false;
			if (strcmp(comandi[i], ">") == 0)
			{
				path_stdout=strdup(comandi[i+1]);
				caso=true;
				c=i+1;
			}
		
			if ((strcmp(comandi[i], "2>") == 0))
			{
				path_stderr=strdup(comandi[i+1]);
				caso=true;
				c=i+1;
			}
		
			if ((strcmp(comandi[i], "<") == 0))
			{
				path_stdin=strdup(comandi[i+1]);
				caso=true;
				c=i+1;
			}
			
			if((caso == false) && i != c)
			{
				args[k]=strdup(comandi[i]);
				/*printf(args[k]);
				printf("\n");*/
				k++;
			}
		}
		
		i=k;
		for (i = k; i<MAX_ARGS; i++)
		{
			args[i]=NULL;
		}
		
		/*for (j = 0; j<MAX_ARGS; j++)
		{
			if(args[j] != NULL) printf(args[j]);
			printf("\n");
		}*/
		
		//fine costruzione args
		
		pid = fork();

        if ( pid == -1 ) {
            perror("fork");
            exit(1);
        }
        if ( pid == 0 ) {
            /* applica l'eventuale redirezione dei canali nel figlio */
            if (path_stdin != NULL) {
                fd = open(path_stdin, O_RDONLY);
                close(STDIN);
                dup(fd);
            }
            if (path_stderr != NULL) {
                fd = open(path_stderr, O_WRONLY|O_CREAT|O_TRUNC, 0666);
                close(STDERR);
                dup(fd);
            }
            if (path_stdout != NULL) {
                fd = open(path_stdout, O_WRONLY|O_CREAT|O_TRUNC, 0666);
                close(STDOUT);
                dup(fd);
            }

            execvp(args[0], args);
            fprintf(stderr, "Errore nell'esecuzione di '%s'\n", args[0]);
            exit (2);
        } else if (bkg == true){
            wait(NULL);
         	getchar();   
        }
        /* libero la memoria allocata dinamicamente nel padre */
        if (path_stdin != NULL) free(path_stdin);
        if (path_stderr != NULL) free(path_stderr);
        if (path_stdout != NULL) free(path_stdout);
        i = 0;
        while (args[i] != NULL)
            free(args[i++]);
		
		/*pid_t pid;
		pid = fork();

        if ( pid == -1 ) {
            perror("fork");
            printf("PD!\n");
            exit(1);
        }
        if (pid == 1)
        {
        	int fd;
        	if (path_stdout != NULL){
        		fd=open(path_stdout, O_WRONLY|O_CREAT|O_TRUNC, 0666);
        		close(STDOUT);
        		dup(fd);
        	}
        	
        	if (path_stdin != NULL){
        		fd=open(path_stdin, O_RDONLY);
        		close(STDIN);
        		dup(fd);
        	}
        	
        	if (path_stderr != NULL){
        		fd=open(path_stderr, O_WRONLY|O_CREAT|O_TRUNC, 0666);
        		close(STDERR);
        		dup(fd);
        	}
        	printf("yeeeeeeeeee\n");
        	execvp(args[0], args);
        	fprintf(stderr, "Errore nell'esecuzione di '%s'\n", args[0]);
        	exit(2);
        }
        else if (!bkg) wait(NULL);
        
        if (path_stdin != NULL) free(path_stdin);
        if (path_stderr != NULL) free(path_stderr);
        if (path_stdout != NULL) free(path_stdout);
        i = 0;
        while (args[i] != NULL)
            free(args[i++]);*/
	}
	exit(0);
}

























