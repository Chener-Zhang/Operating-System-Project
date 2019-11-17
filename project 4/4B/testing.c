#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <strings.h>
#include "struct.h"





int main(){

    struct Direction *dir =(struct Direction*) malloc(sizeof(struct Direction));    
    


    strcpy(dir->next_entry_direction[3]->name,"hello");
    printf("%s;\n",dir->next_entry_direction[3]->name);
    

    
    for (int i = 0; i < direction_list_number; i++)
    {
        printf("%s;\n",dir->next_entry_direction[i]->name);
        if(strcmp(dir->next_entry_direction[i]->name,"hello") == 0){
            printf("yes\n");
        }

    }
    
    
    
    free(dir);

    return 0;
}