/*
 * @project Soundwave's task manager(todo)
 * @file main.c
 * @author SoundWave
 * @date January 23, 2020
 * @brief Main file of project with main loop
 * */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/swtdfile.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <pwd.h>

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


//global variables:
#define SWTD_DATA_SIZE 512
#define SWTD_ONE_DATA_TOKEN_SIZE 1024
char **data;
int len;
int current_data_size;
//!global variables



//init:
#define SWTD_CURRENT_HOME_DIRECTORY "/root/.local/share/" //work only on root user, i need to add "any users" features
#define SWTD_CURRENT_DIRECTORY "swtd"
#define SWTD_CURRENT_TARGET_FILE "soundwave_todo_file.swtd"
void init(void){
	len =0;
	current_data_size = SWTD_DATA_SIZE;
	data = malloc(current_data_size * sizeof(char*));
	if(!data){
		fprintf(stderr, "swtd: allocating memory error");
		exit(EXIT_FAILURE);
	}
	//get user home directory:
	//struct passwd *pw = getpwuid(getuid());
	//char *homedir = pw->pw_dir;
	//check directory:
	struct stat s;
	int err = stat(SWTD_CURRENT_HOME_DIRECTORY "" SWTD_CURRENT_DIRECTORY, &s);
	if(err == -1){
		if(ENOENT==errno){
			//does not exist
			//create:
			int mkdir_status = mkdir(SWTD_CURRENT_HOME_DIRECTORY SWTD_CURRENT_DIRECTORY,0700);
			if(mkdir_status == -1){
				fprintf(stderr, "swtd: create directory error");
				exit(EXIT_FAILURE);
			}
			//create_file:
			FILE *fp = fopen(SWTD_CURRENT_HOME_DIRECTORY SWTD_CURRENT_DIRECTORY "/" SWTD_CURRENT_TARGET_FILE,"w+");
			if(fp == NULL){
				fprintf(stderr, "swtd: file creating error");
				exit(EXIT_FAILURE);
			}
		}
		else{
			perror("stat");
			exit(EXIT_FAILURE);
		}
	}
	else{
		if(S_ISDIR(s.st_mode)){
			//dir exist
			//try to open file:
			FILE *fp = fopen(SWTD_CURRENT_HOME_DIRECTORY""SWTD_CURRENT_DIRECTORY"/"SWTD_CURRENT_TARGET_FILE,"r");
			if(!fp){
				//file don't exist, create file:
				fp = fopen(SWTD_CURRENT_HOME_DIRECTORY""SWTD_CURRENT_DIRECTORY"/"SWTD_CURRENT_TARGET_FILE,"W");
				if(!fp){
					fprintf(stderr,"swtd: can't create file");
					exit(EXIT_FAILURE);
				}
				return;
			} 
			struct swtd_file swfile = read_file(fp);
			if(swfile.len > current_data_size){	
				current_data_size = swfile.len;
				data = realloc(data,current_data_size * sizeof(char*));
				if(!data){
					fprintf(stderr, "swtd: allocating memory error");
					exit(EXIT_FAILURE);
				}
			}
			len = swfile.len;
			if(len!=0){
				for(int i=0;i<=swfile.len;i++){
					data[i] = swfile.data[i];
				}
			}

		}
		else{
			//exist, but not a dir
			fprintf(stderr,"swtd: directory %s can't be created",SWTD_CURRENT_HOME_DIRECTORY""SWTD_CURRENT_DIRECTORY);
			exit(EXIT_FAILURE);
		}
	}

}
//!init


char *create_token_from_args(char **args){
	//char *token = malloc(SWTD_DATA_TOKEN_SIZE * sizeof(char));
	int len = 0;
	for(int i =1;i< strlen(args); i++){
		len+=strlen(args[i]);
	}
	len++;
	char *token = malloc(sizeof(char) * len);
	if(!token){
		fprintf(stderr, "swtd: allocating memory error");
		exit(EXIT_FAILURE);
	}
	if(i==1){
		cprint("empty string");
	}
	if(i==2){
		strcpy(token, args[1]);
		return token;
	}
	strcpy(token, args[1]);
	for(int i =2;i< strlen(args);i++){
		strcat(token, args[i]);
	}
	return token;

}

//list of swtd commands:
char *swtd_commands[] = {
	"help",
	"add",
	"rm",
	"exit"
}
//declarations of swtd commands:
int swtd_help(char **);
int swtd_add(char **);
int swtd_exit(char **);
int swtd_rm(char **);


int swtd_num_commands(){return sizeof(swtd_commands)/sizeof(char*)};

//functions implementation:
int swtd_help(char **args){
	sprint("......\n");
	return 1;
}
int swtd_exit(char **args){return 0;}
int swtd_add(char **args){
	if(len>=current_data_size){
		current_data_size+=SWTD_DATA_SIZE;
		data = realloc(data, current_data_size);
	}
	char *token = create_token_from_args(args);
	data[len] = token;
	len++;
	//addtofile(); add new token to file
	return 1;
}


//read_sine:
#define SWTD_RL_BUFSIZE 1024
char *read_line(void){
	int i =0;
	int current_buffer_size = SWTD_RL_BUFSIZE;
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
#define DELIM " \t\r\n\a"
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
int execute_command(char** args){return 1;}

void task_loop(void){
	char *greeting_line = "hello";
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
	}while(status);
}

int main(){
	//task_loop();
	init();
	for(int i = 0;i <  len;i++){
		printf("%s\n", data[i]);
	}
	return 0;
}
