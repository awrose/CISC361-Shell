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

  token = strtok(commandline, " ");

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

      free(owd);
      free(pwd);
    }else if(strcmp(args[0], "pwd") == 0){
      printf("Printing pwd......\n");
      printf("[%s]\n", pwd);

    }else if(strcmp(args[0], "pid") == 0){
      printf("Printing pid....\n");
      printf("%d\n", getpid());

    }else if(strcmp(args[0], "where") == 0){
      if(argsct == 1){
        printf("Too little args\n");
      }else if(argsct == 2){
        where(args[1], pathlist);
      }else{
        printf("too many arguments");
      }

    }else if(strcmp(args[0], "which") == 0){
      if(argsct == 1){
        printf("Too little args\n");
      }else if(argsct == 2){
        which(args[1], pathlist);
      }else{
        printf("too many arguments");
      }

    }else if(strcmp(args[0], "cd") == 0){
      if(argsct == 1){
        chdir(homedir);

          if ( (pwd = getcwd(NULL, PATH_MAX+1)) == NULL )
          {
            perror("getcwd");
            exit(2);
          }
          owd = calloc(strlen(pwd) + 1, sizeof(char));
          memcpy(owd, pwd, strlen(pwd));
      }else if(args[1][0] == '-'){

        chdir("..");

          if ( (pwd = getcwd(NULL, PATH_MAX+1)) == NULL )
          {
            perror("getcwd");
            exit(2);
          }
          owd = calloc(strlen(pwd) + 1, sizeof(char));
          memcpy(owd, pwd, strlen(pwd));
      }else{
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

    }else if(access(args[0], X_OK) == 0){
      pid_t pid1 = fork();
      if(pid1 == 0){
        execve(args[0], args, environ);
      }else{
        waitpid(pid1, NULL, 0);
      }
    }else{
      while(pathlist){
        if(strstr(pathlist->element, args[0])){
          char *newPathlist = strtok(pathlist, "/");

          while(newPathlist){
            if(access(newPathlist, X_OK) == 0){
              pid_t pid2 = fork();
              if(pid2 == 0){
                execve(newPathlist, args, environ);
              }else{
                waitpid(pid2, NULL, 0);
              }
              break;
            }
            newPathlist = strtok(NULL, "/");
          }
        }

        pathlist = pathlist->next;
      }

      printf("[%s]Command Not Found\n", args[0]);
    }
  }
  }
  return 0;
} /* sh() */

void which(char *command, struct pathelement *pathlist )
{
   struct pathelement *tmp = pathlist;

   while(tmp){
    if(strstr(tmp->element, command)){
      printf("%s\n", tmp->element);
      break; 
    }
    tmp = tmp->next;
   }

}

void where(char *command, struct pathelement *pathlist )
{
  while(pathlist){
    if(strstr(pathlist->element, command)){
      printf("%s\n", pathlist->element);
    }
    pathlist = pathlist->next;
  }
}


void list ( char *currDir )
{
  DIR* dir = opendir(currDir);
  if(!dir){
    perror("opendir");
    exit(EXIT_FAILURE);
  }
  struct dirent* dr;
  while((dr = readdir(dir))){
    printf("%s\n", dr->d_name);
  }
  closedir(dir);
}