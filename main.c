#include "sh.h"
#include <signal.h>
#include <stdio.h>

#define BUFFERSIZE 128
void sig_handler(int signal); 

int main( int argc, char **argv, char **envp )
{
  /* put signal set up stuff here */

	//run a loop until "exit" is entered 
	char buffer[BUFFERSIZE];
	char *userInput = " ";
	int len;

	char *userInputArr;

	while(strcmp(userInput, "exit") != 0){
		printf("still running\n");

		//print [cwd]

		setbuf(stdin, NULL);
		if(fgets(buffer, BUFFERSIZE, stdin)!=NULL){
			len = (int)strlen(buffer);
			buffer[len - 1] = '\0';
			userInput = (char *)malloc(len);
			strcpy(userInput, buffer);
		
			char *parsedInput = strtok(userInput, " ");
			//printf("%s\n", userInput); 
			
			int tmp = 0;
			parsedInput = strtok(NULL, " ");
			
				
		}
	//}
	free(userInput);
	//free(parsedInput);
	//free(command);
	}

	printf("exiting program...\n");
}

void sig_handler(int signal)
{
  /* define your signal handler */
}

