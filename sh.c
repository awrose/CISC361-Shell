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

  //if(argsct > 0){
    if(strcmp(args[0], "exit") == 0){
      printf("Executing built-in [exit]\n");
      go = 0; 

      free(owd);
      free(pwd);
    }else if(strcmp(args[0], "pwd") == 0){
      printf("Executing built-in [pwd]\n");
      printf("[%s]\n", pwd);

      //free(args);

    }else if(strcmp(args[0], "pid") == 0){
      printf("Executing built-in [pid]\n");
      printf("%d\n", getpid());

      //free(args);

    }else if(strcmp(args[0], "where") == 0){
      if(argsct == 1){
        printf("Too little args\n");
      }else if(argsct == 2){
        printf("Executing built-in [where]\n");
        where(args[1], pathlist);
      }else{
        printf("too many arguments\n");
      }

      //free(args);

    }else if(strcmp(args[0], "which") == 0){
      if(argsct == 1){
        printf("Too little args\n");
      }else if(argsct == 2){
        printf("Executing built-in [which]\n");
        which(args[1], pathlist);
      }else{
        printf("too many arguments\n");
      }

      //free(args);

    }else if(strcmp(args[0], "cd") == 0){
      if(argsct == 1){
        printf("Executing built-in [cd]\n");
        chdir(homedir);

          if ( (pwd = getcwd(NULL, PATH_MAX+1)) == NULL )
          {
            perror("getcwd");
            exit(2);
          }
            owd = calloc(strlen(pwd) + 1, sizeof(char));
            memcpy(owd, pwd, strlen(pwd));
      }else if(args[1][0] == '-'){
        printf("Executing built-in [cd]\n");

        chdir("..");

          if ( (pwd = getcwd(NULL, PATH_MAX+1)) == NULL )
          {
            perror("getcwd");
            exit(2);
          }

            owd = calloc(strlen(pwd) + 1, sizeof(char));
            memcpy(owd, pwd, strlen(pwd));
      }else{
        printf("Executing built-in [cd]\n");
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
      printf("Executing built-in [list]\n");
      if(argsct == 1){
        list(owd);
      }else{
        for(int i = 1; i<argsct; i++){
          printf("\n%s:", args[i]);
          list(args[i]);
        }
      }

    }else if(strcmp(args[0], "kill") == 0){
      printf("Executing built-in [kill]\n");
      if(argsct == 2){
        pid_t killpid = atoi(args[1]);
        kill(killpid, SIGTERM);

      }else if(argsct == 3){
        pid_t killpid = atoi(args[2]);
        int signal = -1 * atoi(args[1]);

        kill(killpid, signal);
      }


    }else if(strcmp(args[0], "prompt") == 0){
      printf("Executing built-in [prompt]\n");
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
        printf("Executing built-in [printenv]\n");
        while(*s){
          printf("%s\n", *s++);
        }
      }else if(argsct == 2){
        printf("Executing built-in [printenv]\n");
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
        printf("Executing built-in [setenv]\n");
        while(*s){
          printf("%s\n", *s++);
        }
      }else if(argsct == 2){
        printf("Executing built-in [setenv]\n");
        setenv(args[1], "/", 1);
        if(strcmp(args[1], "HOME") == 0){
          homedir = getenv("HOME");
        }else if(strcmp(args[1], "PATH") == 0){
          free(pathlist->element);
          //free the pathlist
          pathlist = get_path();
        }
      }else if(argsct == 3){
        printf("Executing built-in [setenv]\n");
        setenv(args[1], args[2], 1);

        if(strcmp(args[1], "HOME") == 0){
          homedir = getenv("HOME");
        }else if(strcmp(args[1], "PATH") == 0){
          //freepathlist
          pathlist = get_path();
        }
      }else{
        //add right error message
        printf("Error: too many args\n");
      }

      //freeargs

    }else if(strchr(commandline, "*")!=NULL || strchr(commandline, '?') != NULL){
      wordexp_t p;
      char **w;
      int pos; 

      wordexp(commandline, &p, 0);
      w = p.we_wordv; 

      for(int i = 0; i<p.we_wordc; i++){
        printf("%s\n", w[i]);
      }
      wordfree(&p);
      exit(EXIT_SUCCESS);


    }else if(access(args[0], X_OK) == 0){
      pid_t pid1 = fork();
      if(pid1 == 0){
        printf("Executing [%s]\n", args[0]);
        execve(args[0], args, environ);
      }else{
        waitpid(pid1, NULL, 0);
      }
    }else{
      char *absPath = malloc(200 *sizeof(char));
      while(pathlist){

        sprintf(absPath, "%s/%s", pathlist->element, args[0]);
        if(access(absPath, X_OK) == 0){
          pid_t pid2 = fork();

          if(pid2 == 0){
            printf("Executing [%s]\n", args[0]);
            execve(absPath, args, environ);
          }else{
            waitpid(pid2, NULL, 0);
          }
        }
                pathlist = pathlist->next;
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