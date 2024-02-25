/*****
* Project 04: Shell
* COSC 208, Introduction to Computer Systems, Fall 2023
******/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>// for chdir, fork, execvp
#include <sys/wait.h>// for waitpids


#define PROMPT "shell> "
#define MAX_BG_PROCS 10

//STUFF FOR BACKGROUND PROCESSES
pid_t bg_procs[MAX_BG_PROCS];
int bg_count = 0;

/*
 * Break input string into an array of strings.
 * @param input the string to tokenize
 * @param delimiters the characters that delimite tokens
 * @return the array of strings with the last element of the array set to NULL
 */
char** tokenize(const char *input, const char *delimiters) {    
    char *token = NULL;

    // make a copy of the input string, because strtok
    // likes to mangle strings.  
    char *input_copy = strdup(input);

    // find out exactly how many tokens we have
    int count = 0;
    for (token = strtok(input_copy, delimiters); token; 
            token = strtok(NULL, delimiters)) {
        count++ ;
    }
    free(input_copy);

    input_copy = strdup(input);

    // allocate the array of char *'s, with one additional
    char **array = (char **)malloc(sizeof(char *)*(count+1));
    int i = 0;
    for (token = strtok(input_copy, delimiters); token;
            token = strtok(NULL, delimiters)) {
        array[i] = strdup(token);
        i++;
    }
    array[i] = NULL;
    free(input_copy);
    return array;
}

/*
 * Free all memory used to store an array of tokens.
 * @param tokens the array of tokens to free
 */
void free_tokens(char **tokens) {    
    int i = 0;
    while (tokens[i] != NULL) {
        free(tokens[i]); // free each string
        i++;
    }
    free(tokens); // then free the array
}

void add_bg_proc(pid_t pid)
{
    if(bg_count < MAX_BG_PROCS)
    {
        bg_procs[bg_count++] = pid;
    }
    else
    {
        fprintf(stderr, "ERROR: TOO MANY BACKGROUND PROCESSES\n");
    }
}

void remove_bg_proc(pid_t pid)
{
    for(int i = 0; i < bg_count; i++)
    {
        if(bg_procs[i] == pid)
        {
            for(int j = i; j < bg_count - 1; j++)
            {
                bg_procs[j] = bg_procs[j+1];
            }
            bg_count--;
            return;
        }
    }
}

/*
* CONSULTED CHAT GPT FOR THIS METHOD
*/
void sigchld_handler(int sig) 
{
    pid_t pid;
    int status;
    while((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        remove_bg_proc(pid);
    }
}

/*
* Will be invoked when the shell recieves a SIGINT signal
*/
void sigint_handler(int sig)
{
    //printf("\n%s + 5", PROMPT);
    printf("\n");
    //fflush(stdout);
}

int main(int argc, char **argv) { 
    signal(SIGCHLD, sigchld_handler);

    printf("%s", PROMPT); // Display the prompt at the start
    fflush(stdout);

    char buffer[1024];
    while (fgets(buffer, 1024, stdin) != NULL) {
        char **command = tokenize(buffer, " \t\n");
        if (command[0] == NULL  || command[0][0] == '#') {
            // Do nothing
        }
        else 
        {
            if(strcmp(command[0], "exit") == 0)
            {
                break;
            }

            else if(strcmp(command[0], "cd") == 0)
            {
                if(command[1] == NULL) //there is no directory to move to
                {
                    fprintf(stderr, "CD IS EXPECTING AN ARGUMENT\n");
                }

                else
                {
                    //printf("\nthis is command 1: %s \n", command[1]);
                    if(chdir(command[1]) != 0)
                    {
                        perror("CD");
                    }
                }
            }

            else if(strcmp(command[0], "fg") == 0)
            {
                if(bg_count > 0)
                {
                    int status;
                    waitpid(bg_procs[bg_count -1], &status, 0);
                    remove_bg_proc(bg_procs[bg_count-1]);
                }
                else
                {
                    fprintf(stderr, "NO BACKGROUND PROCESSES\n");
                }
            }

            else 
            {
            int i = 0;
            int background = 0;

            while(command[i] != NULL) i++;
            if (i > 0 && strcmp(command[i-1], "&") == 0) {
                background = 1;
                free(command[i-1]);
                command[i-1] = NULL;
            }

            if(background && bg_count > 0) {
                fprintf(stderr, "ERROR: BACKGROUND PROCESS IS ALREADY RUNNING\n");
            } else {
                pid_t pid = fork();

                if(pid == -1) {
                    perror("FORK");
                } else if(pid == 0) {
                    if(execvp(command[0], command) == -1) {
                        perror("EXECVP");
                        exit(EXIT_FAILURE);
                    }
                } else {
                    if(!background) {
                        int status;
                        waitpid(pid, &status, 0);
                    } else {
                        add_bg_proc(pid);
                        printf("STARTED BACKGROUND PROCESS PID: %d\n", pid);
                    }
                }
            }
            free_tokens(command);
        }

        printf("%s", PROMPT);
        fflush(stdout);
        }
    }
    printf("\nexit\n");
    return EXIT_SUCCESS;
}