#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <strings.h>
#include "struct.h"




// initilization of direction struct 
int init_dir(struct Direction *dir, char name[]){
    strcpy(dir->name, name);
    dir->current_index = -1;  

    for (int i = 0; i < direction_list_number; i++)
    {
                
        memset(&dir->next_entry_direction[i]->name[0], 0, sizeof(dir->next_entry_direction[i]->name));
        dir->next_entry_direction_tracking[i] = 0;
    }

    for (size_t i = 0; i < file_list_number; i++)
    {
        // working;
        
        dir->file_list_tracking[i] = 0;
    }
    return 0;
}


int main(){

    struct Direction *dir =(struct Direction*) malloc(sizeof(struct Direction));    
    
    init_dir(dir,"hello");
    
    for (int i = 0; i < direction_list_number; i++)
    {
        printf("%s;\n",dir->next_entry_direction[i]->name);
      

    }
    
    
    
    free(dir);

    return 0;
}