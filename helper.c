/**
* 	CS374 : Operating Systems I
* 	Programming Assignment 4: SMALLSH
* 
*	Elaine Zamora
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>

#define INPUT_LENGTH 2048
#define MAX_ARGS 512
#define MAX_BG 100

// prototypes
void checkBgProcesses();


// Global variables to track the shell's status and background processes
int lastForegroundStatus = 0;
int backgroundAllowed = 1; // 1 = normal, 0 = foreground-only mode (toggled by SIGTSTP)
pid_t backgroundPids[MAX_BG];
int backgroundCount = 0;

/*
*	struct command_line
*	Source: Provided via SMALLSH project description
*/
struct command_line{
	char *argv[MAX_ARGS + 1];
	int argc;
	char *input_file;
	char *output_file;
	bool is_bg;
};

void checkBgProcesses() { return; }

/*
*	parse_input()
*
*	Input:  Empty struct to populate via user input
*	Objective: Populates struct using user input
*
*	Source: Skeleton provided via SMALLSH project description
*
*/
struct command_line *parse_input()
{
	char input[INPUT_LENGTH];
	struct command_line *curr_command = (struct command_line *) calloc(1, sizeof(struct command_line));

	// get input
	printf(": ");
	fflush(stdout);

	if (fgets(input, INPUT_LENGTH, stdin) == NULL) {
        printf("\n");
        exit(0); // Exit shell if EOF/CTRL-D happens - test this
    }

	// Strip trailing newline character
    input[strcspn(input, "\n")] = '\0';

	// If it's a completely blank line or a comment line, return the empty struct
    if (strlen(input) == 0 || input[0] == '#') {
        return curr_command;
    }

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

	// add NULL to indicate end of line 
	curr_command->argv[curr_command->argc] = NULL;

    // turn off background execution if SIGTSTP mode is active
    if (curr_command->is_bg && backgroundAllowed == 0) {
        curr_command->is_bg = false;
    }
	return curr_command;
}


/*
*	executeOtherCommand()
*
*	Input: command_line struct
* 	Objective: Attempts to execute user defined command
*
*	Source: Modeled off code in Exploration: Process API - creating and
*		terminating processes
*
*/
void executeOtherCommand(struct command_line *curr_command){
	pid_t spawnpid = fork();

	switch (spawnpid){
		case -1:
			perror("Fork failed!");
			lastForegroundStatus = 1;	// mark as failed
			exit(EXIT_FAILURE);
		case 0: 
			// fork successful, use execvp to confirm cmd is valid in PATH
			execvp(curr_command->argv[0], curr_command->argv);

			perror("Invalid command entered!");
			exit(EXIT_FAILURE);
		default:
			// parent waits for child to finish
			waitpid(spawnpid, &lastForegroundStatus, 0);
			break;

	}
}



int main()
{

	struct command_line *curr_command;

	while(true){
		// background process check 
		checkBgProcesses();

		// prompt user for new cmd
		curr_command = parse_input();

		// runs if the user typed something, didn't immediately hit enter
		if (curr_command->argc > 0) {
			// CHECK BUILT IN-S FIRST
			
			// check for exit
			if (strcmp(curr_command->argv[0], "exit") == 0) {
				exit(EXIT_SUCCESS); 
			} 	
			// check for cd
			else if (strcmp(curr_command->argv[0], "cd") == 0) {
				// if user types 'cd' without dir, use getenv("HOME")
				char *targetDir = curr_command->argv[1]; 
				if (targetDir == NULL) {
					targetDir = getenv("HOME");
				}	
				if (chdir(targetDir) != 0) {
					perror("cd failed");
				}
			}
			// check for status
			else if (strcmp(curr_command->argv[0], "status") == 0){
				// check if process exited normally or was killed
				if (WIFEXITED(lastForegroundStatus)) {
					printf("exit value %d\n", WEXITSTATUS(lastForegroundStatus));
				} else {
					printf("terminated by signal %d\n", WTERMSIG(lastForegroundStatus));
				}
				fflush(stdout); 
			}
			else{
				executeOtherCommand(curr_command);
			}

		}
		/****************  memory leak cleanup  ****************/

		// free mem allocated for string duplicates
		for (int i = 0; i < curr_command->argc; i++) {
        	if (curr_command->argv[i] != NULL) {
            	free(curr_command->argv[i]);
        	}
    	}
		// free mem allocated for in/out file data
		if (curr_command->input_file != NULL) {
			free(curr_command->input_file);
		}
		if (curr_command->output_file != NULL) {
			free(curr_command->output_file);
		}
	
		// free mem allocated for struct/mem pointers to data
		free(curr_command);
		
	}
	return EXIT_SUCCESS;
}
