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
#include "get_path.h"

int pid;
int sh( int argc, char **argv, char **envp);
void which(char *command, struct pathelement *pathlist );
void where(char *command, struct pathelement *pathlist );
void list ( char *dir );
void printenv(char **envp);


#define PROMPTMAX 32
#define MAXARGS 10
