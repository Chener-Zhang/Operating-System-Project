#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <strings.h>
#include <errno.h>

#include "struct.h"


int main(int argc, char **argv)
{
    char cmd[20] = "hello";
    printf("%s\n",cmd);
    return 0;
    
}


int parsing(){
    char cmd[20];
    char arg[20];
    char copy[20];
    char user_input[20];
    printf(">"); // print ">" 
    fgets(user_input,20,stdin); // get user_input
    strcpy(copy,user_input); // copy just in case    
    strcpy(cmd,strtok(user_input," ")); // assign first arg -> cmd;
    printf("[%s]\n", cmd);
    strcpy(arg,strtok(NULL," \n"));
    printf("[%s]\n",arg);
    return 0;
}