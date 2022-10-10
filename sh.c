#include "sh.h"

#define BUFFERSIZE 128

//TODO: clean code 
//TODO: SIGNAL STUFF 
//TODO: MAKE SURE everything works
//TODO: Free all allocated memory 

extern char **environ;

int sh( int argc, char **argv, char **envp )
{
  char *prompt = calloc(PROMPTMAX, sizeof(char));
  char *commandline = calloc(MAX_CANON, sizeof(char));
  char *token, *pwd, *owd;
  char **args;
  int uid, argsct, go = 1;
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
    /*EXIT: will exit the program when "exit" is typed, regardless of the input after "exit"
    frees all dynamically allocated variables*/
    if(strcmp(args[0], "exit") == 0){
      printf("Executing built-in [exit]\n");
      go = 0; 

      for(int i = 0; i<argsct; i++){
        free(args[i]);
      }

      free(args);

      /*PWD: prints the current working directory, will ignore any arguments typed after*/
    }else if(strcmp(args[0], "pwd") == 0){
      printf("Executing built-in [pwd]\n");
      printf("[%s]\n", pwd);

      for(int i = 0; i<argsct; i++){
        free(args[i]);
      }

      free(args);
      //free(args);
      /*PID: prints the process's PID, will ignore arguments typed after "pid"*/
    }else if(strcmp(args[0], "pid") == 0){
      printf("Executing built-in [pid]\n");
      printf("%d\n", getpid());

      for(int i = 0; i<argsct; i++){
        free(args[i]);
      }

      free(args);

      //free(args);
      /*WHERE: Will locate a program file in the user's path everywhere that it appears
      if ran with no arguments --> won't do anything, ran any commands, will find the entire 
      lists' commands and will be printed under the [cmd]:*/
    }else if(strcmp(args[0], "where") == 0){
      if(argsct == 1){
        printf("where: too few arguments\n");

        free(args[0]);
        free(args);
      }else{
        printf("Executing built-in [where]\n");
        for(int i = 1; i<argsct; i++){
          printf("%s:\n", args[i]);
          where(args[i], pathlist);
        }

      for(int i = 0; i<argsct; i++){
        free(args[i]);
      }

      free(args);
      }

      //free(args);

      /*WHICH: will locate a program file specified in the user's path 
      if no arguments: will not do anything, with arguments: will find the path for 
      each argument, if more than one arg: will print the [cmd]: before the paths*/
    }else if(strcmp(args[0], "which") == 0){
      if(argsct == 1){
        printf("which: too few arguments\n");

        free(args[0]);
        free(args);
      }else{
        printf("Executing built-in [which]\n");
        for(int i = 1; i<argsct; i++){
          printf("%s:\n", args[i]);
          which(args[i], pathlist);
        }

      for(int i = 0; i<argsct; i++){
        free(args[i]);
      }

      free(args);
      }

      //free(args);
      /*CD: will change the working directory
      no args: change the directory to the HOME directory
      '-': change the directory to the previous directory
      any other: will attempt to change to that directory if it exists */
    }else if(strcmp(args[0], "cd") == 0){
      if(argsct == 1){
        if(chdir(homedir) != 0){
          perror("chdir");
        }else{
          printf("Executing built-in [cd]\n");
        }

          if ( (pwd = getcwd(NULL, PATH_MAX+1)) == NULL )
          {
            perror("getcwd");
            exit(2);
          }

        free(args[0]);
        free(args);

      }else if(args[1][0] == '-'){
        if(chdir("..") != 0){
          perror("chdir");
        }else{
          printf("Executing built-in [cd]\n");
        }

          if ( (pwd = getcwd(NULL, PATH_MAX+1)) == NULL )
          {
            perror("getcwd");
            exit(2);
          }

        for(int i = 0; i<argsct; i++){
          free(args[i]);
        }

        free(args);
      }else{
        if(chdir(args[1]) != 0){
          perror("chdir");
        }else{
          printf("Executing built-in [cd]\n");
        }
            if ( (pwd = getcwd(NULL, PATH_MAX+1)) == NULL )
          {
            perror("getcwd");
            exit(2);
          }

        for(int i = 0; i<argsct; i++){
          free(args[i]);
        }

        free(args);
      }

      //free(args);
      /*LIST:
      no args: list the files in the cwd one per line
      args: list the files in each directory followed by [name of the directory]: */
    }else if(strcmp(args[0], "list") == 0){
      if(argsct == 1){
        list(owd);

        free(args[0]);
        free(args);
      }else{
        for(int i = 1; i<argsct; i++){
          printf("\n%s:", args[i]);
          list(args[i]);
        }

        for(int i = 0; i<argsct; i++){
          free(args[i]);
        }
        free(args);
      }
      /*KILL: kills a process
      args: 
      - Just a pid --> send kill that process 
      - pid + signal --> send SIGTERM to that process with the signal*/
    }else if(strcmp(args[0], "kill") == 0){
      if(argsct > 2){
        printf("Executing built-in [kill]\n");
        if(argsct == 2){
          pid_t killpid = atoi(args[1]);
          kill(killpid, SIGTERM);

        }else if(argsct == 3){
          pid_t killpid = atoi(args[2]);
          int signal = -1 * atoi(args[1]);

          kill(killpid, signal);
        }

        for(int i = 0; i<argsct; i++){
          free(args[i]);
        }
        free(args);

      }else{
        free(args[0]);
        free(args);
      }

      /*PROMPT changes the prompt in the shell (the prefix)
      no args: will prompt user to enter a prompt
      --> after entering the string, it will add it to the beginning of the prompt 
      with arg: will set the prefix of the prompt to the argument
      User can't input a sentence (anything with spaces) for the prompt
      */
    }else if(strcmp(args[0], "prompt") == 0){
      printf("Executing built-in [prompt]\n");
      if(argsct == 1){
        printf("please input a prompt\n");
        	if(fgets(buffer, BUFFERSIZE, stdin)!=NULL){
		        len = (int)strlen(buffer);
		        buffer[len-1] = '\0';
		        strcpy(prompt, buffer);
	        }
      }else if(argsct > 2){
        printf("prompt: too many arguments\n");
      }else{ 
        prompt = args[1];
      }

        for(int i = 0; i<argsct; i++){
          free(args[i]);
        }
        free(args);
      /*PRINTENV: Prints the environment 
      No args: print whole environment
      One argument: call getEnv() on that environment 
      two + arguments: error message */
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
        printf("printenv: too many args\n");

      }

      for(int i = 0; i<argsct; i++){
        free(args[i]);
      }
      free(args);

      /*SETENV: 
      no args: print the entire environment 
      one arg: set that arg as an empty environment variable
      two args: second arg is the value of the first
      more args: error 
      PATH --> update linked list for the path directories 
      HOME --> cd should now go to the new HOME directory*/
    }else if(strcmp(args[0], "setenv") == 0){
      char **s = environ; 

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

      for(int i = 0; i<argsct; i++){
        free(args[i]);
      }
      free(args);


      /*WILDCARD: truly have no idea why this works, followed the example on wordexp(3)*/
    }else if(strchr(commandline, '*')!=NULL || strchr(commandline, '?') != NULL){
      wordexp_t p;
      char **w;
      //int pos; 


      wordexp(commandline, &p, 0);
      w = p.we_wordv; 

      for(int i = 0; i<p.we_wordc; i++){
        printf("%s\n", w[i]);
      }
      wordfree(&p);

      for(int i = 0; i<argsct; i++){
        free(args[i]);
      }
      free(args);

    /*IF ABSOLUTE PATH, test if the */
    }else if(access(commandline, X_OK) == 0){
      pid_t pid1 = fork();
      if(pid1 == 0){
        printf("Executing [%s]\n", commandline);
        execve(commandline, args, environ);
      }else{
        waitpid(pid1, NULL, 0);
      }

      for(int i = 0; i<argsct; i++){
        free(args[i]);
      }
      free(args);
      /*NOT ABSOLUTE PATH: search for command, run the executable*/
    }else{
      int flag1 = 1;
      char *absPath = malloc(200 *sizeof(char));
      struct pathelement *tmp = pathlist;
      while(tmp){

        //sprintf(absPath, "%s/%s", tmp->element, args[0]);
        if(access(absPath, X_OK) == 0){
          flag1 = 0;
          pid_t pid2 = fork();

          if(pid2 == 0){
            printf("Executing [%s]\n", commandline);
            execve(absPath, args, environ);
            break;
          }else{
            waitpid(pid2, NULL, 0);
          }
        }
                tmp = tmp->next;
      }

      if(flag1){
        printf("%s: command not found\n", commandline);
      }

      for(int i = 0; i<argsct; i++){
        free(args[i]);
      }
      free(args);
      }
  }else{
    free(args);
  }
  }

  //free(pathlist->element);
  freePathlist(pathlist);
  free(commandline);
  free(prompt);

  return 0;
}


/*WHICH: 
Args: command, pathlist
Creates a new variable: find, store the current pathlist + command in find
IF access returns 0, it is in the pathlist, print the path
break once the first path is found
*/
void which(char *command, struct pathelement *pathlist )
{
  char *find = malloc(200*sizeof(char));
  int flag = 1;

  while(pathlist != NULL){
    sprintf(find, "%s/%s", pathlist->element, command);
    if(access(find, X_OK) == 0){
      flag = 0;
      printf("%s/%s\n", pathlist->element, command);

      break;
    }

    pathlist = pathlist->next; 
  }

  if(flag){
    perror("access");
  }

  free(find);

}

/*WHERE: 
Args: command, pathlist
Creates a new variable: find, store the current pathlist + command in find
IF access returns 0, it is in the pathlist, print the path
will print every path that the command is found in
*/
void where(char *command, struct pathelement *pathlist )
{
  char *find = malloc(200 *sizeof(char));
  int flag = 1; 

  while(pathlist){
    sprintf(find, "%s/%s", pathlist->element, command);
    if(access(find, X_OK) == 0){
      flag = 0;
      printf("%s/%s\n", pathlist->element, command);
    }

    pathlist = pathlist->next; 
  }

  if(flag){
    perror("access");
  }

  free(find);
}

/*LIST: takes in a directory, and prints all of the files in that directory one by one on a line 
if the directory doesn't exist, it will display an error message*/
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

void freePathlist(struct pathelement *pathlist){
  struct pathelement *tmp; 

  while(pathlist){
    tmp = pathlist;
    pathlist = pathlist->next;
    free(tmp);
  }

  free(pathlist);
}