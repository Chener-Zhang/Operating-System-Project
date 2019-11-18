#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <strings.h>
#include <errno.h>

#include "struct.h"

char command[20];
char argument[20];

int main(int argc, char **argv)
{
    char cmd[20] = "hello";
    printf("%s\n",cmd);
    return 0;
    
}


int parsing(){

    char user_input[20];
    printf(">"); // print ">" 
    fgets(user_input,20,stdin); // get user_input    
    strcpy(command,strtok(user_input," ")); // assign first arg -> cmd;
    printf("[%s]\n", command);
    strcpy(argument,strtok(NULL," \n"));
    printf("[%s]\n",argument);
    return 0;
}

int char_reset(){
    memset(command, 0, sizeof(command)); 
    memset(argument, 0, sizeof(command)); 
    return 0;
}