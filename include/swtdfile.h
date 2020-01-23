/* 
 * @project Soundwave's task manager(todo)
 * @file swtdfile.h
 * @author Soundwave
 * @date January 23, 2020
 * @brief file description
 * */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct swtd_file{
	char **data;
	int len;
};
#define SWTD_READ_FILE_BUFFER_SIZE 1024
#define SWTD_TOKEN_SIZE 256
#define SWTD_TOKENS_ARRAY_SIZE 1024
struct swtd_file read_file(FILE *file){
	if(file == NULL){
		fprintf(stderr, "swtd: can't open the file");
	}
	else{
		int current_buffer_size = SWTD_READ_FILE_BUFFER_SIZE;
		char *buffer = malloc(current_buffer_size * sizeof(char));
		int current_tokens_size = SWTD_TOKENS_ARRAY_SIZE;
		char **tokens = malloc(current_tokens_size * sizeof(char*));
		int tokens_iterator = 0;
		if(!buffer){
			fprintf(stderr, "swtd: allocating memory error");
			exit(EXIT_FAILURE);
		}
		if(!tokens){
			fprintf(stderr, "swtd: allocating memory error");
			exit(EXIT_FAILURE);
		}
		while(fgets(buffer, 1024, file)){
			current_buffer_size+=SWTD_READ_FILE_BUFFER_SIZE;
			buffer = realloc(buffer, current_buffer_size * sizeof(char));
		}
		for(int i = 0;i< current_buffer_size; ++i){
			if(buffer[i] == '{'){
				//start get token:
				char *token;
				int current_token_size = SWTD_TOKEN_SIZE;
				token = malloc(current_token_size * sizeof(char));
				if(!token){
					fprintf(stderr, "SWTD: allocating memory error");
					exit(EXIT_FAILURE);
				}
				int token_i = 0;
				i++;
				while(buffer[i]!='}'){
					if(i >= current_buffer_size){
						fprintf(stderr, "swtd: read file error");
						exit(EXIT_FAILURE);
					}
					token[token_i] = buffer[i];
					if(token_i >= current_token_size){
						current_token_size += SWTD_TOKEN_SIZE;
						token = realloc(token, current_token_size * sizeof(char));
						if(!token){
							fprintf(stderr, "SWTD: allocating memory error");
							exit(EXIT_FAILURE);
						}
					}
					token_i++;
					i++;
				}
				tokens[tokens_iterator] = token;
				tokens_iterator++;
				if(tokens_iterator>=current_tokens_size){
					current_tokens_size +=SWTD_TOKENS_ARRAY_SIZE;
					tokens = realloc(tokens, current_tokens_size * sizeof(char*));
					if(!tokens){
						fprintf(stderr, "swtd: allocating memory error");
						exit(EXIT_FAILURE);
					}
				}

			}
		} 
		free(buffer);
		if(tokens_iterator == 0){
			//empty file:
			free(tokens);
			struct swtd_file ret_file;
			ret_file.data = NULL;
			ret_file.len = 0;
			return ret_file;
		}
		struct swtd_file ret_file;
		ret_file.data = tokens;
		ret_file.len = tokens_iterator;
		return ret_file;
	}
}



void addtofile(FILE *file, char* token){
	if(file!=NULL){
		fprintf(file,"{%s}",token);
	}
	else{
		fprintf(stderr,"swtd: can't open file");
	}
}
