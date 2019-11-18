#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <strings.h>
#include "struct.h"



int my_shell(){
    char user_input[20];
    gets(user_input);
    printf(">\n");
    printf("you have enter \n [%s]\n",user_input);

    if(strcmp(user_input,"quit")== 0){        
            exit(0);
    }

    return 0;
}

int main(){


    while(1){

        my_shell();

    
    }
    return 0;
}