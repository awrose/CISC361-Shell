#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <pwd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "sh.h"

#define BUFFERSIZE 128

int sh( int argc, char **argv, char **envp )
{
  char *prompt = calloc(PROMPTMAX, sizeof(char));
  char *commandline = calloc(MAX_CANON, sizeof(char));
  char *command, *arg, *commandpath, *p, *pwd, *owd;
  char **args = calloc(MAXARGS, sizeof(char*));
  int uid, i, status, argsct, go = 1;
  struct passwd *password_entry;
  char *homedir;
  struct pathelement *pathlist;

  uid = getuid();
  password_entry = getpwuid(uid);               /* get passwd info */
  homedir = password_entry->pw_dir;		/* Home directory to start
						  out with*/
     
  if ( (pwd = getcwd(NULL, PATH_MAX+1)) == NULL )
  {
    perror("getcwd");
    exit(2);
  }
  owd = calloc(strlen(pwd) + 1, sizeof(char));
  memcpy(owd, pwd, strlen(pwd));
  prompt[0] = ' '; prompt[1] = '\0';

  /* Put PATH into a linked list */
  pathlist = get_path();

  char buffer[BUFFERSIZE];
  int len;
	

  while ( go )
  {
	//prompt
	printf("\n [%s]>\n", owd);
	printf("Enter a command: \n");	

	//get command line
	setbuf(stdin, NULL);
	if(fgets(buffer, BUFFERSIZE, stdin)!=NULL){
		len = (int)strlen(buffer);
		buffer[len-1] = '\0';
		strcpy(commandline, buffer);
	}

	//command = strtok(commandline, " ");
	
	arg = strtok(commandline, " ");	
	command = commandline;
	arg = strtok(NULL, " ");

    /* check for each built in command and implement */
	if(strcmp(command, "exit") == 0){
		//free all allocated memory
		go = 0;
	}else if(strcmp(command, "pwd")==0){
		printf("Printing Working Directory....\n");
		pwd = getcwd(NULL, 0);
		printf("%s\n", pwd);
		free(pwd);
		//printf("Printing Working Directory.....\n%s\n", owd);
	}else if(strcmp(command, "pid")==0){
		int pid = getpid();
		printf("Printing pid.....\n%d\n", pid);
	}else if(strcmp(command, "which") == 0){
		//which
	}else if(strcmp(command, "where") == 0){
		//where
	}else if(strcmp(command, "cd") == 0){
		//cd
	}else if(strcmp(command, "list") == 0){
		//list
	}else if(strcmp(command, "kill") == 0){
		//kill
	}else if(strcmp(command, "prompt") == 0){
		//prompt
	}else if(strcmp(command, "printenv") == 0){
		//printenv
	}else if(strcmp(command, "setenv") == 0){
		//setenv
	}
		


     /*  else  program to exec */
    {
       /* find it */
       /* do fork(), execve() and waitpid() */

      /* else */
        /* fprintf(stderr, "%s: Command not found.\n", args[0]); */
    }
  }
  return 0;
} /* sh() */

char *which(char *command, struct pathelement *pathlist )
{
   /* loop through pathlist until finding command and return it.  Return
   NULL when not found. */

} /* which() */

char *where(char *command, struct pathelement *pathlist )
{
  /* similarly loop through finding all locations of command */
} /* where() */

void list ( char *dir )
{
  /* see man page for opendir() and readdir() and print out filenames for
  the directory passed */
} /* list() */

