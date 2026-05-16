/**
 * A sample program for parsing a command line. If you find it useful,
 * feel free to adapt this code for Assignment 4.
 * Do fix memory leaks and any additional issues you find.
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>	// used for exec

#define INPUT_LENGTH 2048
#define MAX_ARGS		 512


struct command_line
{
	char *argv[MAX_ARGS + 1];
	int argc;
	char *input_file;
	char *output_file;
	bool is_bg;
};


struct command_line *parse_input()
{
	char input[INPUT_LENGTH];
	struct command_line *curr_command = (struct command_line *) calloc(1, sizeof(struct command_line));

	// Get input
	printf(": ");
	fflush(stdout);
	fgets(input, INPUT_LENGTH, stdin);

	// Tokenize the input
	char *token = strtok(input, " \n");
	while(token){
		if(!strcmp(token,"<")){
			curr_command->input_file = strdup(strtok(NULL," \n"));
		} else if(!strcmp(token,">")){
			curr_command->output_file = strdup(strtok(NULL," \n"));
		} else if(!strcmp(token,"&")){
			curr_command->is_bg = true;
		} else{
			curr_command->argv[curr_command->argc++] = strdup(token);
		}
		token=strtok(NULL," \n");
	}
	return curr_command;
}


/* printLs()
*
* Description: uses exec to execute "ls" command
*
*
*/
void printLs(void)
{
char *newargv[] = { "/bin/ls", NULL };
	execv(newargv[0], newargv);

	/* exec returns only on error */
	perror("execv");
	exit(EXIT_FAILURE);

}

/* killMyProcesses()
*
* 	Description: 
*				should i take as input the current path?
*	
*/


/* BUILT IN COMMAND: userExit()
*
* Description:
*		When the user enters "exit" command, exits the shell. 
*		It takes no arguments.
*		When this command is run, the shell kills:
*			- Any other processes or jobs that the shell has started 
*			before it terminates itself.
*
*/


/* BUILT IN COMMAND: userCd()
*
* 	Description: changes the working directory of smallsh
*
*	Input: Can take 0 or 1 argument
*			- No argument will change to the directory
*				specified in the HOME environment variable
*			- Argument:  the path of a directory to change to
*	Supports absolute and relative paths
*
*/


/* BUILT IN COMMAND: userStatus()
*
* 	Description:  prints out either the exit status or the terminating 
*		signal of the last foreground process ran by the shell
*
*	If this command is run before any foreground command is run, then it 
*		must simply return the exit status 0.
*
*	The three built-in shell commands do not count as foreground processes 
*		for the purposes of this built-in command - i.e., status must ignore 
*		built-in commands.
*
*/


/* 
*
* Description:
*
*
*/


/* 
*
* Description:
*
*
*/


int main()
{
	struct command_line *curr_command;

	while(true)
	{
		curr_command = parse_input();
		//if (curr_command->input_file == NULL){continue;}	// CHECK ME

		// do 3 string compares? 

	}

	return EXIT_SUCCESS;
}


/*
*
*	functions :
*				killProcess()
*
*				
*
*
*
*
*
*
*
*/