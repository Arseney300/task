#include <iostream>
#include <string>
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

char *greeting_line = "Soundwave's simple task manager";
char *end_line = "command list:    add     rm      exit";

void cprint(char *line){
	system("clear"); //only unix
	//default information
	printf(CYN"\t\t%s\n"RESET,greeting_line);
	printf("%s",line);
	//printf("%s\n",end_line);
}

