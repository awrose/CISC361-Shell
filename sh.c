#include "sh.h"

#define BUFFERSIZE 128

int sh( int argc, char **argv, char **envp )
{
  char *prompt = calloc(PROMPTMAX, sizeof(char));
  char *commandline = calloc(MAX_CANON, sizeof(char));
  char *command, *arg, *token, *commandpath, *p, *pwd, *owd;
  char **args;
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
	printf("%s [%s]>", prompt, pwd);

	//get command line
	setbuf(stdin, NULL);
	if(fgets(buffer, BUFFERSIZE, stdin)!=NULL){
		len = (int)strlen(buffer);
		buffer[len-1] = '\0';
		strcpy(commandline, buffer);
	}

  char *token = strtok(commandline, " ");

  //find the length
  int count = 0; 
  while(token != NULL){
    count++;
    token = strtok(NULL, " ");
  }

  args = malloc((count+1) *sizeof(char*));
  args[count]=0;

  strcpy(commandline, buffer);
  count = 0; 
  token = strtok(buffer, " ");
  while(token != NULL){
    args[count] = (char*)malloc((strlen(token)+1) *sizeof(char*));
    strcpy(args[count], token);
    count++;
    token = strtok(NULL, " ");
  }
  argsct = count;

  if(argsct > 0){
    if(strcmp(args[0], "exit") == 0){
      printf("Exiting Program.....\n");
      go = 0; 

      free(args[0]);
      free(args);
      free(owd);
      free(pwd);
    }else if(strcmp(args[0], "pwd") == 0){
      printf("Printing pwd......\n");
      printf("[%s]\n", pwd);

      free(args[0]);
      free(args);
    }else if(strcmp(args[0], "pid") == 0){
      printf("Printing pid....\n");
      printf("%d\n", getpid());

      free(args[0]);
      free(args);
    }else if(strcmp(args[0], "where") == 0){

    }else if(strcmp(args[0], "which") == 0){

    }else if(strcmp(args[0], "cd") == 0){

    }else if(strcmp(args[0], "list") == 0){

    }else if(strcmp(args[0], "kill") == 0){

    }else if(strcmp(args[0], "prompt") == 0){

    }else if(strcmp(args[0], "printev") == 0){

    }else if(strcmp(args[0], "setenv") == 0){

    }
  }
    /* check for each built in command and implement */
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
