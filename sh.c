#include "sh.h"

#define BUFFERSIZE 128

extern char **environ;

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
        /* check for each built in command and implement */
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
      where(args[1], pathlist);

    }else if(strcmp(args[0], "which") == 0){
      which(args[1], pathlist);

    }else if(strcmp(args[0], "cd") == 0){
      if(argsct == 1){
        //cd to homedirectory 
        chdir(homedir);

          if ( (pwd = getcwd(NULL, PATH_MAX+1)) == NULL )
          {
            perror("getcwd");
            exit(2);
          }
          owd = calloc(strlen(pwd) + 1, sizeof(char));
          memcpy(owd, pwd, strlen(pwd));
      }else if(args[1][0] == '-'){
        //go to the previous directory'

        chdir("..");

          if ( (pwd = getcwd(NULL, PATH_MAX+1)) == NULL )
          {
            perror("getcwd");
            exit(2);
          }
          owd = calloc(strlen(pwd) + 1, sizeof(char));
          memcpy(owd, pwd, strlen(pwd));
      }else{
        //go to the specified directory
        chdir(args[1]);
            if ( (pwd = getcwd(NULL, PATH_MAX+1)) == NULL )
          {
            perror("getcwd");
            exit(2);
          }
          owd = calloc(strlen(pwd) + 1, sizeof(char));
          memcpy(owd, pwd, strlen(pwd));
      }
    }else if(strcmp(args[0], "list") == 0){
      if(argsct == 1){
        list(owd);
      }else{
        for(int i = 1; i<argsct; i++){
          printf("\n%s:", pwd);
          list(args[i]);
        }
      }

    }else if(strcmp(args[0], "kill") == 0){
      if(argsct == 2){
        //kill
        pid_t killpid = atoi(args[1]);
        kill(killpid, SIGTERM);
        free(args[1]);
        free(args[0]);
        free(args);
      }else if(argsct == 3){
        pid_t killpid = atoi(args[2]);
        int signal = -1 * atoi(args[1]);

        kill(killpid, signal);
        free(args[2]);
        free(args[1]);
        free(args[0]);
        free(args);
      }

    }else if(strcmp(args[0], "prompt") == 0){
      if(argsct == 1){
        //prompt for a new string
        printf("please input a prompt\n");
        	if(fgets(buffer, BUFFERSIZE, stdin)!=NULL){
		        len = (int)strlen(buffer);
		        buffer[len-1] = '\0';
		        strcpy(prompt, buffer);
	        }
      }else{ 
        prompt = args[1];
      }

    }else if(strcmp(args[0], "printenv") == 0){
      char **s = environ;

      if(argsct == 1){
        while(*s){
          printf("%s\n", *s++);
        }
      }else if(argsct == 2){
        printf("%s\n", getenv(args[1]));
      }else{
        //add right error message
        printf("Error: too many args\n");

      }

    }else if(strcmp(args[0], "setenv") == 0){
      char **s = environ; 

      //PATH: update linked list for the path directories (free() the old one)
      //HOME: cd with no arguments should now go to the new home

      if(argsct == 1){
        while(*s){
          printf("%s\n", *s++);
        }
      }else if(argsct == 2){
        //set that as an empty environment variable
      }else if(argsct == 3){
        //second one is the value of the first
      }else{
        //add right error message
        printf("Error: too many args\n");
      }

    }
  }
     /*  else  program to exec */
       /* find it */
       /* do fork(), execve() and waitpid() */

      /* else */
        /* fprintf(stderr, "%s: Command not found.\n", args[0]); */
  }
  return 0;
} /* sh() */

void which(char *command, struct pathelement *pathlist )
{
   /* loop through pathlist until finding command and return it.  Return
   NULL when not found. */
   struct pathelement *tmp = pathlist;

   while(tmp){
      if(tmp->element == command){
        printf("%s\n", command);
        break;
      }

      tmp = tmp->next;

   }

} /* which() */

void where(char *command, struct pathelement *pathlist )
{
  /* similarly loop through finding all locations of command */
  struct pathelement *tmp = pathlist;
  while(tmp){
    if(tmp->element == command){
      printf("%s\n", command);
    }
    tmp = tmp->next;
  }
} /* where() */


void list ( char *currDir )
{
  /* see man page for opendir() and readdir() and print out filenames for
  the directory passed */
  DIR* dir = opendir(currDir);
  if(!dir){
    perror("opendir");
    exit(EXIT_FAILURE);
  }
  struct dirent* dr;
  while((dr = readdir(dir))){
    //printf("READING FILES\n");
    printf("%s\n", dr->d_name);
  }
  closedir(dir);
} /* list() */

void freeArgs(char *args, int argsct){
  for(int i = 0; i<argsct; i++){
    free(args[i]);
  }

  free(args);

} /* freeArgs()*/

