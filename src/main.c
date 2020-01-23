/*
 * @project Soundwave's taks manager(todo)
 * @file main.c
 * @author SoundWave
 * @date January 23, 2020
 * @brief Main file of project with main loop
 * */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/swfileformat.h"


//Colors
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"
//!Colors



//init data file:
#define SWTD_CURRENT_HOME_DIRECTORY "~/.local/share/"
#define SWTD_CURRENT_DIRECTORY "swtd"
#define SWTD_CURRENT_TARGET_FILE "soundwave_todo_file.txt"
void init_data_file(void){
	//check directory and file exist
	//and create if not
}
//!init data file




//read_sine:
#define SWTD_RL_BUFSIZE 1024
char *read_line(void){
	int i =0;
	int current_buffer_size = SWSH_RL_BUFSIZE;
	char *buffer = malloc(sizeof(char)* current_buffer_size);
	int c;
	if(!buffer){
		fprintf(stderr, "swtd: allocating memory error\n");
		exit(EXIT_FAILURE);
	}
	while(1){
		c = getchar();
		if(c == EOF || c =='\n'){
			buffer[i] = '\0';
			return buffer;
		}
		else{
			buffer[i] = c;
		}
		//check memory buffer:
		if(i>=current_buffer_size){
			current_buffer_size +=SWTD_RL_BUFSIZE;
			buffer = realloc(buffer, current_buffer_size);
			if(!buffer){
				fprintf(stderr, "swtd: allocating memory error\n");
				exit(EXIT_FAILURE);
			}
		}
		i++;
	}
}

//split line:
#define SWTD_TOKENS_BUFSIZE 1024
#define DELIM " \t\r\n\a";
char **split_line(char *line){
	int current_token_size = SWTD_TOKENS_BUFSIZE;
	int i =0;
	char **tokens = malloc(current_token_size * sizeof(char*));
	char *token;
	if(!tokens){
		fprintf(stderr, "swtd: allocating memory error\n");
		exit(EXIT_FAILURE);
	}
	token = strtok(line, DELIM);
	while(token!=NULL){
		tokens[i] = token;
		i++;
		if(i>=current_token_size){
			current_token_size+=SWTD_TOKENS_BUFSIZE;
			tokens = realloc(tokens, current_token_size*sizeof(char*));
			if(!tokens){
				fprintf(stderr, "swtd: allocating memory error\n");
				exit(EXIT_FAILURE);
			}
		}
		token = strtok(NULL, DELIM);
	}
	tokens[i] = NULL;
	return tokens;
}



void task_loop(void){
	printf("%s",greeting_line);
	char *line;
	char **argline;
	int status;
	do{
		printf(">");
		line = read_line();
		argline = split_line(line);
		status = execute_command(argline);

		free(line);
		free(argline);
	}while(status)
}

int main(){
	task_loop();
	return 0;
}
